#include "pch.h"
#include "render.h"
#include "main.h"
#include "PlayerController.h"
#include "LevelManager.h"
#include "Viewmodel.h"
#include "skybox.h"
#include "EntityList.h"

void DrawRay(ID3D11DeviceContext* pContext, FXMVECTOR Origin, FXMVECTOR Direction, bool bNormalize, FXMVECTOR color)
{
	VertexPositionColor verts[3];
	XMStoreFloat3(&verts[0].position, Origin);

	XMVECTOR NormDirection = XMVector3Normalize(Direction);
	XMVECTOR RayDirection = (bNormalize) ? NormDirection : Direction;

	XMVECTOR PerpVector = XMVector3Cross(NormDirection, g_XMIdentityR1);

	if (XMVector3Equal(XMVector3LengthSq(PerpVector), g_XMZero))
	{
		PerpVector = XMVector3Cross(NormDirection, g_XMIdentityR2);
	}
	PerpVector = XMVector3Normalize(PerpVector);

	XMStoreFloat3(&verts[1].position, XMVectorAdd(RayDirection, Origin));
	PerpVector = XMVectorScale(PerpVector, 0.0625f);
	NormDirection = XMVectorScale(NormDirection, -0.25f);
	RayDirection = XMVectorAdd(PerpVector, RayDirection);
	RayDirection = XMVectorAdd(NormDirection, RayDirection);
	XMStoreFloat3(&verts[2].position, XMVectorAdd(RayDirection, Origin));

	XMStoreFloat4(&verts[0].color, color);
	XMStoreFloat4(&verts[1].color, color);
	XMStoreFloat4(&verts[2].color, color);


	basicEffect->Apply(pContext);
	pContext->IASetInputLayout(inputLayout);
	primitiveBatch->Begin();
	primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, 2);
	primitiveBatch->End();
}


void DrawCube(ID3D11DeviceContext* pContext, CXMMATRIX mWorld, FXMVECTOR color)
{
	static const XMVECTOR s_verts[8] =
	{
		{ -1, -1, -1, 0 },
		{ 1, -1, -1, 0 },
		{ 1, -1, 1, 0 },
		{ -1, -1, 1, 0 },
		{ -1, 1, -1, 0 },
		{ 1, 1, -1, 0 },
		{ 1, 1, 1, 0 },
		{ -1, 1, 1, 0 }
	};
	static const WORD s_indices[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	VertexPositionColor verts[8];
	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR v = XMVector3Transform(s_verts[i], mWorld);
		XMStoreFloat3(&verts[i].position, v);
		XMStoreFloat4(&verts[i].color, color);
	}

	basicEffect->Apply(pContext);
	pContext->IASetInputLayout(inputLayout);

	primitiveBatch->Begin();
	primitiveBatch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, s_indices, _countof(s_indices), verts, 8);
	primitiveBatch->End();
}

void DrawAabb(ID3D11DeviceContext* pContext, const BoundingBox& box, FXMVECTOR color)
{
	XMMATRIX matWorld = XMMatrixScaling(box.Extents.x, box.Extents.y, box.Extents.z);
	XMVECTOR position = XMLoadFloat3(&box.Center);
	matWorld.r[3] = XMVectorSelect(matWorld.r[3], position, g_XMSelect1110);

	DrawCube(pContext, matWorld, color);
}
#include "ParticleSystem.h"
void Renderer(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* pSwapChain)
{
	//pContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//pContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//pContext->RSSetState(m_states->Wireframe());
	//basicEffect->Apply(pContext);
	//pContext->IASetInputLayout(inputLayout);
	//primitiveBatch->Begin();
	//primitiveBatch->DrawTriangle(VertexPositionColor(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow), VertexPositionColor(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow), VertexPositionColor(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow));
	//primitiveBatch->End();


	if (!LevelObjects.size()) LoadLevel(device, context, "e1m1");
	else RenderLevel(context);

	if (Sky && SkyboxTexture) RenderSky(context);
	else CreateSky(device, context, "sky1");

	RenderEntityList(device, context);

	particle_system::RenderParticles(device, context);


#ifdef _DEBUG
	basicEffect->SetWorld(m_world);
	basicEffect->SetView(m_view);
	basicEffect->SetProjection(m_proj);

	DrawAabb(context, player::PlayerCollider, Colors::Red);
	XMVECTOR Direction = XMVectorScale(player::PlayerVelocity, 10.0f);
	DrawRay(context, player::PlayerPosition, Direction, false, Colors::Pink);
	XMVECTOR Direction2 = XMVectorScale(player::GroundVel, 10.0f);
	DrawRay(context, player::PlayerPosition, Direction2, false, Colors::Green);
	for (int i = 0; i < LevelBBoxes.size(); i++)
	{
		DrawAabb(context, LevelBBoxes[i], Colors::AliceBlue);
	}

	DrawRay(context, g_PrimaryRay.origin, g_PrimaryRay.direction, false, Colors::Pink);
#endif


	viewmodel::RenderViewmodel(device);
}