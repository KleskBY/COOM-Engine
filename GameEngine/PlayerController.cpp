#include "pch.h"
#include "PlayerController.h"
#include "main.h"
#include "InputManager.h"
#include "LevelManager.h"

namespace player
{

	/*******************************************************/
	/********************** MOUSE **************************/
	/*******************************************************/

	void ClampInputAngles()
	{
		// limit pitch to straight up or straight down
		constexpr float limit = XM_PIDIV2 - 0.01f;
		m_pitch = std::max(-limit, m_pitch);
		m_pitch = std::min(+limit, m_pitch);

		// keep longitude in sane range by wrapping
		if (m_yaw > XM_PI) m_yaw -= XM_2PI;
		else if (m_yaw < -XM_PI) m_yaw += XM_2PI;
	}

	void MouseLook()
	{
		ClampInputAngles();

		float y = sinf(m_pitch);
		float r = cosf(m_pitch);
		float z = r * cosf(m_yaw);
		float x = r * sinf(m_yaw);

		XMVECTOR lookAt = m_cameraPos + Vector3(x, y, z);
		m_view = XMMatrixLookAtRH(m_cameraPos, lookAt, Vector3::Up);
	}

	/*******************************************************/
	/********************* KEYBOARD ************************/
	/*******************************************************/

	void UpdateColliderPosition()
	{
		PlayerCollider.Center = PlayerPosition;
		PlayerCollider.Extents = PlayerSize / 2.0f;
	}

	float lastJump;
	void PlayerJump()
	{
		if (lastJump + 0.2f > Time) return;
		lastJump = Time;
		PlayerGrounded = false;
		PlayerVelocity.y = PlayerVelocity.y + JUMP_FORCE;
	}

	void ApplyFriction()
	{
		if (PlayerVelocity.x || PlayerVelocity.z)
		{
			float f = 1 - DeltaTime * FRICTION;
			PlayerVelocity.x = PlayerVelocity.x * f;
			PlayerVelocity.z = PlayerVelocity.z * f;

			//PlayerVelocity.x = Lerp(PlayerVelocity.x, 0.f, Friction * DeltaTime);
			//PlayerVelocity.z = Lerp(PlayerVelocity.z, 0.f, Friction * DeltaTime);
		}
	}
	void ApplyAcceleration()
	{
		Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, 0.f, 0.f);
		Vector3 move = Vector3::Transform(input::Move, q);
		move.y = 0.f;
		move.Normalize();
		PlayerVelocity = PlayerVelocity + move * ACCELERATION * SPEED * DeltaTime;

		//PlayerVelocity.x = Lerp(PlayerVelocity.x, move.x, Accelerate * DeltaTime);
		//PlayerVelocity.z = Lerp(PlayerVelocity.z, move.z, Accelerate * DeltaTime);
		//PlayerVelocity = PlayerVelocity + move * Accelerate * DeltaTime;
	}


	// Function to determine the collision face but sometimes it returns broken vector idk why
	DirectX::XMVECTOR GetCollisionNormal(const DirectX::BoundingBox& box, const DirectX::XMVECTOR& collisionPoint)
	{
		DirectX::XMVECTOR center = (box.Center + box.Extents);

		// Calculate the differences between the collision point and the center along each axis
		DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(collisionPoint, center);
		DirectX::XMVECTOR absDiff = DirectX::XMVectorAbs(diff);

		// Find the minimum absolute difference (the axis with the smallest difference)
		float minDiff = std::min({ DirectX::XMVectorGetX(absDiff), DirectX::XMVectorGetY(absDiff), DirectX::XMVectorGetZ(absDiff) });

		// Determine the collision normal based on the axis with the smallest difference
		if (minDiff == DirectX::XMVectorGetX(absDiff))
		{
			return DirectX::XMVectorSet(DirectX::XMVectorGetX(diff) > 0 ? 1.0f : -1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (minDiff == DirectX::XMVectorGetY(absDiff))
		{
			return DirectX::XMVectorSet(0.0f, DirectX::XMVectorGetY(diff) > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
		}
		else
		{
			return DirectX::XMVectorSet(0.0f, 0.0f, DirectX::XMVectorGetZ(diff) > 0 ? 1.0f : -1.0f, 0.0f);
		}
	}

	bool GetCollisionNormal2(BoundingBox levelObject, Vector3 pos, Vector3& collisionNormal)
	{
		g_PrimaryRay.origin = pos;
		g_PrimaryRay.direction = pos.Forward;
		float distForward = 99999.f;
		if (!levelObject.Intersects(pos, pos.Forward, distForward)) distForward = 99999.f;
		float distBack = 99999.f;
		if (!levelObject.Intersects(pos, -pos.Forward, distBack)) distBack = 99999.f;
		float distLeft = 99999.f;
		if (!levelObject.Intersects(pos, -pos.Right, distLeft)) distLeft = 99999.f;
		float distRight = 99999.f;
		if (!levelObject.Intersects(pos, pos.Right, distRight)) distRight = 99999.f;
		float min = std::min({ distForward, distLeft, distRight, distBack });
		if (min < 3.f)
		{
			if (min == distForward) collisionNormal = Vector3(0, 0, 1);
			if (min == distBack) collisionNormal = Vector3(0, 0, -1);
			if (min == distLeft) collisionNormal = Vector3(1, 0, 0);
			if (min == distRight) collisionNormal = Vector3(-1, 0, 0);
			return true;
		}
		return false;
	}

	bool IsObjectToLeft(const XMVECTOR& referenceVector, const XMVECTOR& objectPosition)
	{
		// Calculate the vector pointing from the reference to the object
		XMVECTOR objectVector = objectPosition - referenceVector;

		// Get the cross product of the referenceVector and objectVector
		XMVECTOR crossProduct = XMVector3Cross(referenceVector, objectVector);

		// Check the direction of the cross product to determine left or right
		return XMVectorGetY(crossProduct) > 0.0f; // If greater than 0, object is to the left, else to the right
	}

	void GetCameraDirectionsFromYawPitch(float yaw, float pitch, XMVECTOR& forward, XMVECTOR& left, XMVECTOR& right, XMVECTOR& back)
	{
		// Calculate the forward direction
		forward = XMVectorSet(
			cosf(yaw) * cosf(pitch),
			sinf(pitch),
			sinf(yaw) * cosf(pitch),
			0.0f
		);

		// Calculate the left direction
		left = XMVectorSet(
			cosf(yaw + XM_PI * 0.5f),
			0.0f,
			sinf(yaw + XM_PI * 0.5f),
			0.0f
		);

		// Calculate the right direction
		right = XMVectorSet(
			cosf(yaw - XM_PI * 0.5f),
			0.0f,
			sinf(yaw - XM_PI * 0.5f),
			0.0f
		);

		// Calculate the back direction (opposite of forward)
		back = -forward;
	}



	Vector3 lastGoodPos = Vector3::Zero;
	bool wasGrounded = false;
	void CheckCollisions()
	{
		if (PlayerGrounded && !wasGrounded) PlayerGrounded = false;
		if (PlayerGrounded) wasGrounded = false;

		bool badPos = false;
		float lastHeight = -999999.f;
		for (const auto& levelObject : LevelBBoxes)
		{
			if (PlayerCollider.Intersects(levelObject))
			{
				float groundHeight = levelObject.Center.y + levelObject.Extents.y;
				float playerBottom = PlayerCollider.Center.y - PlayerCollider.Extents.y;

				if (playerBottom + STEP_HEIGHT > groundHeight) //collision with ground
				{
					if (groundHeight > lastHeight)
					{
						lastHeight = groundHeight;

						PlayerPosition.y = groundHeight + PlayerSize.y / 2;
						PlayerGrounded = true;
						wasGrounded = true;
						UpdateColliderPosition();
					}
				}
				else
				{
					if (lastGoodPos == Vector3::Zero) lastGoodPos = PlayerPosition + Vector3::Up * DeltaTime;
					Vector3 shit = lastGoodPos - PlayerPosition;
					shit.Normalize();

					Vector3 collisionNormal = GetCollisionNormal(levelObject, PlayerPosition);					
					if (!GetCollisionNormal2(levelObject, PlayerPosition, collisionNormal))
					{
						if (!GetCollisionNormal2(levelObject, PlayerPosition + PlayerSize * 0.49f, collisionNormal))
						{
							GetCollisionNormal2(levelObject, PlayerPosition - PlayerSize * 0.49f, collisionNormal);
						}
					}

					Vector3 GroundVel2 = PlayerVelocity - collisionNormal * PlayerVelocity.Dot(collisionNormal) /* pls.Length()*/;
					PlayerVelocity = Vector3(GroundVel2.x, PlayerVelocity.y, GroundVel2.z);

					badPos = true;
					PlayerPosition = lastGoodPos + shit * 0.1f;
					PlayerPosition = PlayerPosition + collisionNormal * 10.f * DeltaTime;
					UpdateColliderPosition();
					break;
				}
			}
		}
		if (!badPos) lastGoodPos = PlayerPosition;
	}


	void PlayerMove()
	{
		if (PlayerHealth <= 0) return;
		if (noclip)
		{
			Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);
			Vector3 move = Vector3::Transform(input::Move, q);
			PlayerPosition += move * NOCLIP_SPEED * DeltaTime;
			PlayerVelocity = Vector3::Zero;
		}
		else
		{
			ApplyFriction();
			ApplyAcceleration();


			if (PlayerGrounded)
			{
				if (lastJump + 0.1f < Time) PlayerVelocity.y = 0;
				if (input::Move.y > 0) PlayerJump();
			}
			else PlayerVelocity.y = PlayerVelocity.y - GRAVITY * DeltaTime;
			PlayerVelocity.Clamp(-MAX_VELOCITY, MAX_VELOCITY);

			if (input::Move.y < 0.f) PlayerSize = Vector3::Lerp(PlayerSize, CROUCH_SIZE, 10.f * DeltaTime);
			else PlayerSize = Vector3::Lerp(PlayerSize, STAND_SIZE, 10.f * DeltaTime);

			CheckCollisions();
			PlayerPosition += PlayerVelocity * 120.f * DeltaTime;
		}
		UpdateColliderPosition();
		if(Vector3::Distance(m_cameraPos, PlayerPosition) > 6.f) m_cameraPos = m_cameraPos.Lerp(m_cameraPos, PlayerPosition + VecView, 16.0f * DeltaTime);
		m_cameraPos = m_cameraPos.Lerp(m_cameraPos, PlayerPosition + VecView, 16.0f * DeltaTime); //trick  //m_cameraPos = PlayerPosition + VecView;
	}
}


/*

					Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);
					DirectX::XMVECTOR rotationQuaternion = DirectX::XMLoadFloat4(&q);
					Vector3 forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
					forward = DirectX::XMVector3Rotate(forward, rotationQuaternion);
					Vector3 right = Vector3::Right;
					right = -DirectX::XMVector3Rotate(right, rotationQuaternion);

				//Vector3 collisionNormal = levelObject.Center - PlayerCollider.Center;
				//collisionNormal.Normalize();
				//PrintVector(PlayerVelocity - collisionNormal * PlayerVelocity.Dot(collisionNormal));


				//GroundVel = Vector3::Reflect(shit, collisionNormal) * 0.25f;
				//GroundVel.y = 0.f;

				//PlayerVelocity = PlayerVelocity + GroundVel;
				//PlayerPosition = lastGoodPos;




				////Good but buggy
				{
					//Vector3 vec1 = ReflectVectorAt90Degrees(PlayerPosition, lastGoodPos);
					//vec1.y = 0.0f;
					//vec1.Normalize();

					//Vector3 vec2 = ReflectVectorAt90Degrees(PlayerPosition, PlayerVelocity);
					//vec2.y = 0.0f;
					//vec2.Normalize();
					//PlayerVelocity = vec1 * 0.1f + vec2 * 0.1f;

					//PlayerVelocity = ReflectVectorAt90Degrees(PlayerPosition, lastGoodPos);
					////PlayerVelocity = ReflectVectorAt90Degrees(PlayerPosition, PlayerVelocity);
					//PlayerVelocity.Normalize();
					//PlayerVelocity.y = 0;
					//PlayerVelocity = PlayerVelocity * 0.1f;
				}


				//Working but stuck in the middle
				{
					//PlayerPosition = lastGoodPos;
					//Vector3 collisionNormal = levelObject.Center - PlayerCollider.Center;
					//collisionNormal.Normalize();
					//PrintVector(collisionNormal);
					//Vector3 pls = PlayerVelocity;
					//pls.Normalize();
					//GroundVel = pls - collisionNormal * pls.Dot(collisionNormal);
					//PlayerVelocity = PlayerVelocity - GroundVel * 0.3f;
				}

				//MEME push back player
				{
					//Vector3 bbmin = levelObject.Center - levelObject.Extents;
					//Vector3 bbmax = levelObject.Center + levelObject.Extents;
					//Vector3 dir = bbmin - bbmax;

					//float maxX = bbmax.x + PlayerSize.x / 1.7f;
					//float maxZ = bbmax.z + PlayerSize.z / 1.7f;

					//float minX = bbmin.x - PlayerSize.x / 1.7f;
					//float minZ = bbmin.z - PlayerSize.z / 1.7f;

					//Vector3 collisionNormal = levelObject.Center - PlayerCollider.Center;
					//collisionNormal = XMVector3Normalize(collisionNormal);
					//PrintVector(collisionNormal);
					//if (levelObject.Extents.x < levelObject.Extents.z)
					//{
					//	if (collisionNormal.x > 0.1f) PlayerPosition.x = minX;
					//	else if (collisionNormal.x < -0.1f) PlayerPosition.x = maxX;
					//}
					//else
					//{
					//	if (collisionNormal.z > 0.1f) PlayerPosition.z = minZ;
					//	else if (collisionNormal.z < -0.1f) PlayerPosition.z = maxZ;
					//}
				}




*/