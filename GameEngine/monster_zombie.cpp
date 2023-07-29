#include "pch.h"
#include "monster_zombie.h"
#include "main.h"
#include "PlayerController.h"
#include "EntityList.h"
#include "LevelManager.h"
#include "render.h"

float STEP_HEIGHT = 0.5f;
float GRAVITY = 0.800f;
float FRICTION = 25.0f;
float MOVE_SPEED = 2.0f;

void monster_zombie_think()
{
	bool visible = true;

	Vector3 dir = m_cameraPos - self->position;
	dir.Normalize();


	float playerDist;
	if (player::PlayerCollider.Intersects(self->position, dir, playerDist))
	{
		float dist;
		for (int i = 0; i < LevelBBoxes.size(); i++)
		{
			if (LevelBBoxes[i].Intersects(self->position, dir, dist))
			{
				if (dist < playerDist)
				{
					visible = false;
					break;
				}
			}
		}
	}


	if (visible)
	{
		dir.y = 0;
		//self->position = self->position + dir * 2.0f * DeltaTime;
		self->velocity = self->velocity + dir * MOVE_SPEED;
	}

	//Friction
	if (self->velocity.x || self->velocity.z)
	{
		float f = 1 - DeltaTime * FRICTION;
		self->velocity.x = self->velocity.x * f;
		self->velocity.z = self->velocity.z * f;
	}


	BoundingBox collider;
	collider.Center = self->position;
	collider.Extents = self->size / 2;

	bool grounded = false;
	float lastHeight = -999999.f;
	for (int i = 0; i < LevelBBoxes.size(); i++)
	{
		if (LevelBBoxes[i].Intersects(collider))
		{
			float groundHeight = LevelBBoxes[i].Center.y + LevelBBoxes[i].Extents.y;
			if (groundHeight > lastHeight)
			{
				lastHeight = groundHeight;
				if (groundHeight - STEP_HEIGHT < self->position.y)
				{
					self->position.y = groundHeight + self->size.y / 2;
					grounded = true;
				}
			}
		}
	}

	if (!grounded)
	{
		self->position.y = self->position.y - 1 * DeltaTime;
	}


	self->position = self->position + self->velocity * DeltaTime;


	//g_PrimaryRay.direction = Vector3::Down;
	//g_PrimaryRay.origin = self->position;
	//float groundDist;
	//float maxDist = 99.f;
	//for (int i = 0; i < LevelBBoxes.size(); i++)
	//{
	//	if (LevelBBoxes[i].Intersects(self->position, Vector3::Down, groundDist))
	//	{
	//		if (groundDist < maxDist)
	//		{
	//			maxDist = groundDist;
	//			printf("%f\n", groundDist);
	//			self->position.y = LevelBBoxes[i].Center.y + (LevelBBoxes[i].Extents.y / 2) + self->size.y / 2;
	//		}
	//	}
	//}



	//self->position.y = self->position.y + 1.f * DeltaTime;
	//printf("%f\n", self->position.y);
}
