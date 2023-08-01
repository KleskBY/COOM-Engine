#pragma once

namespace player
{
	void MouseLook();
	void PlayerMove();
	//Settings
	constexpr float FRICTION = 8.0f;
	constexpr float ACCELERATION = 1.00f;
	constexpr float SPEED = 0.5f;
	constexpr Vector3 MAX_VELOCITY = Vector3(0.1f, 0.7f, 0.1f);
	constexpr float GRAVITY = 0.800f;
	constexpr float JUMP_FORCE = 0.12f;
	constexpr float STEP_HEIGHT = 0.5f;
	constexpr float NOCLIP_SPEED = 30.0f;
	constexpr Vector3 STAND_SIZE = Vector3(1.0f, 1.8f, 1.0f);
	constexpr Vector3 CROUCH_SIZE = Vector3(1.0f, 1.0f, 1.0f);
	//constexpr float FRICTION = 8.0f;
	//constexpr float ACCELERATION = 1.00f;
	//constexpr float SPEED = 50.0f;
	//constexpr Vector3 MAX_VELOCITY = Vector3(1000.f, 2000.f, 1000.f);
	//constexpr float GRAVITY = 20.f;
	//constexpr float JUMP_FORCE = 5.0f;
	//constexpr float STEP_HEIGHT = 0.5f;
	//constexpr float NOCLIP_SPEED = 300.0f;



	//Variables
	inline Vector3 PlayerPosition = Vector3(-1.0f, 2.0f, 0.0f);
	inline Vector3 PlayerVelocity;
	inline Vector3 VecView = Vector3(0.0f, 0.6f, 0.0f);
	inline Vector3 CameraPos;
	inline Vector3 PlayerSize = STAND_SIZE;
	inline bool PlayerGrounded;
	inline bool noclip;


	//
	inline BoundingBox PlayerCollider;
	inline Vector3 GroundVel;
	inline std::string Viewmodel;

}