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
					badPos = true;
					PlayerPosition = lastGoodPos;

					Vector3 collisionNormal = levelObject.Center - PlayerCollider.Center;
					collisionNormal.y = 0.f;
					collisionNormal.Normalize();

					Vector3 pls = Vector3(PlayerVelocity.x, 0.0f, PlayerVelocity.z);
					pls.Normalize();
					GroundVel = pls - collisionNormal * pls.Dot(collisionNormal) /* pls.Length()*/;
					GroundVel.Normalize();
					PlayerVelocity = -GroundVel * 2.0f * DeltaTime;

					//Vector3 pls = Vector3(PlayerVelocity.x, 0.0f, PlayerVelocity.z);
					//pls.Normalize();
					//GroundVel = pls - collisionNormal * pls.Dot(collisionNormal) * pls.Length();
					//GroundVel.Normalize();
					//PlayerVelocity = PlayerVelocity - GroundVel * 0.3f;
				}
			}
		}
		if (!badPos) lastGoodPos = PlayerPosition;
	}

	void PlayerMove()
	{
		if (noclip)
		{
			Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);
			DirectX::XMVECTOR rotationQuaternion = DirectX::XMLoadFloat4(&q);
			DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			forward = DirectX::XMVector3Rotate(forward, rotationQuaternion);
			Vector3 vec = forward;

			Vector3 move = Vector3::Transform(input::Move, q);
			PlayerPosition += move * NOCLIP_SPEED * DeltaTime * 0.1f;
			PlayerVelocity = Vector3::Zero;
		}
		else
		{
			ApplyFriction();
			ApplyAcceleration();

			if (PlayerGrounded)
			{
				PlayerVelocity.y = 0;
				if (input::Move.y) PlayerJump();
			}
			else PlayerVelocity.y = PlayerVelocity.y - GRAVITY * DeltaTime;

			PlayerVelocity.Clamp(-MAX_VELOCITY, MAX_VELOCITY);
			PlayerPosition += PlayerVelocity * 120.f * DeltaTime;
			//PlayerPosition += PlayerVelocity;
			//printf("%f\n", PlayerVelocity.Length());
		}

		UpdateColliderPosition();
		if(!noclip) CheckCollisions();

		m_cameraPos = m_cameraPos.Lerp(m_cameraPos, PlayerPosition + VecView, 16.0f * DeltaTime); //trick  //m_cameraPos = PlayerPosition + VecView;
	}
}


/*



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