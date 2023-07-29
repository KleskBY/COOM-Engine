#include "pch.h"
#include "Client.h"
#include "main.h"
#include "PlayerController.h"
#include "Weapons.h"


namespace client
{
	void ClientThink()
	{
		player::MouseLook();
		player::PlayerMove();

		weapon::WeaponFrame();

		if (GetAsyncKeyState(0x4E))
		{
			player::noclip = !player::noclip;
			while (GetAsyncKeyState(0x4E)) { Sleep(1); }
		}

		if (GetAsyncKeyState(0x02))
		{
			Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);
			DirectX::XMVECTOR rotationQuaternion = DirectX::XMLoadFloat4(&q);
			DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			forward = DirectX::XMVector3Rotate(forward, rotationQuaternion);
			Vector3 vec = forward;
			m_cameraPos = player::PlayerPosition + player::VecView - vec * 10.0f;
		}

		
	}
}