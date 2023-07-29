#pragma once

namespace weapon
{
	constexpr float RECOIL_DECREASE_SPEED = 20.f;

	inline bool AttackHeld;
	inline float RecoilPitch;
	inline float RecoilYaw;
	void Attack();
	void WeaponFrame();
}