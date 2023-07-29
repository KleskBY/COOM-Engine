#include "pch.h"
#include "EntityList.h"
#include "main.h"
#include "dx_tools.h"
#include "PlayerController.h"
#include "LevelManager.h"
#include "monster_zombie.h"
#include "render.h"


void AddMonster(std::string name, Vector3 pos)
{
	ENTITY ent;
	ent.name = name;
	ent.position = pos;
	ent.angles = Vector3::Zero;
	ent.size = Vector3(1.0f, 1.f, 1.f);
	ent.model = "data\\textures\\monsters\\mino.dds";
	ent.think = monster_zombie_think;
	ent.bbox.Center = pos;
	ent.bbox.Extents = ent.size / 2;
	ent.sprite = true;
	EntityList.push_back(ent);
	//EntityList.push_back({ "zombie", pos, Vector3::Zero, Vector3(1.0f, 1.f, 1.f), "zombie" });
}

void SUB_Remove()
{
	EntityList.erase(EntityList.begin() + selfID);
	if(ModelPool[selfID]) ModelPool[selfID].release();
	if(TexturePool[selfID]) TexturePool[selfID].ReleaseAndGetAddressOf();
}

void RenderEntityList(ID3D11Device* device, ID3D11DeviceContext* context)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i].model.size())
		{
			//GeometricPrimitive::VertexCollection cubeVerts;
			//GeometricPrimitive::IndexCollection cubeIndices;
			//GeometricPrimitive::CreateBox(cubeVerts, cubeIndices, EntityList[i].size);
			////for (auto& it : cubeVerts) it.position = it.position + EntityList[i].position;
			//CreateDDSTextureFromFile(device, L"mino.dds", nullptr, m_texture.ReleaseAndGetAddressOf());
			//auto lol = GeometricPrimitive::CreateCustom(context, cubeVerts, cubeIndices);


			//auto lol = GeometricPrimitive::CreateBox(context, EntityList[i].size);
			if (!ModelPool[i])
			{
				ModelPool[i] = GeometricPrimitive::CreateBox(context, EntityList[i].size);
			}
			if (!TexturePool[i])
			{
				std::wstring texture_path(EntityList[i].model.begin(), EntityList[i].model.end());
				if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, TexturePool[i].ReleaseAndGetAddressOf()) != S_OK)
				{
					printf("ERROR LOADING TEXTURE: %s\n", EntityList[i].model.c_str());
					GenerateTexture(device, TexturePool[i].ReleaseAndGetAddressOf());
				}
			}

			if (ModelPool[i] && TexturePool[i])
			{
				Matrix worldMatrix = Matrix::CreateTranslation(EntityList[i].position);
				if (EntityList[i].sprite)
				{
					XMMATRIX rotationMatrix = LookInDirection(EntityList[i].position, m_cameraPos, Vector3::Up);
					worldMatrix = Matrix(rotationMatrix) * worldMatrix;
				}

				m_effect->Apply(context);
				context->IASetInputLayout(m_inputLayout);
				m_effect->SetWorld(worldMatrix);
				m_effect->SetView(m_view);
				m_effect->SetProjection(m_proj);
				m_effect->SetTexture(TexturePool[i].Get());
				ModelPool[i]->Draw(m_effect.get(), m_inputLayout, true, false);
				//lol->Draw(worldMatrix, m_view, m_proj, Colors::White, m_texture.Get());
			}
		}

		EntityList[i].bbox.Center = EntityList[i].position;
		EntityList[i].bbox.Extents = EntityList[i].size / 2;

		selfID = i;
		self = &EntityList[i];
		if (EntityList[i].think != nullptr && EntityList[i].next_think < Time) EntityList[i].think();





			//Quaternion q = Quaternion::CreateFromYawPitchRoll(m_yaw, -m_pitch, 0.f);
			//DirectX::XMVECTOR rotationQuaternion = DirectX::XMLoadFloat4(&q);
			//Vector3 cameraForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			//Vector3 cameraUp = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), q);



			//Vector3 cubePosition = Vector3(0, 1, 0); // Replace with the cube's position
			//Vector3 cubeForward = cameraForward;    // Negative view direction to face the camera

			//// Compute the right and up vectors for the cube's orientation
			//Vector3 cubeRight = cameraUp.Cross(cubeForward);
			//Vector3 cubeUp = cubeForward.Cross(cubeRight);
			//cubeRight.Normalize();
			//cubeUp.Normalize();

			//Matrix cubeTransform(
			//	cubeRight.x, cubeRight.y, cubeRight.z, 0.0f,
			//	cubeUp.x, cubeUp.y, cubeUp.z, 0.0f,
			//	cubeForward.x, cubeForward.y, cubeForward.z, 0.0f,
			//	cubePosition.x, cubePosition.y, cubePosition.z, 1.0f
			//);
			//for (auto& it : cubeVerts)
			//{
			//	it.position = Vector3::Transform(it.position, cubeTransform);
			//}

			//XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f); // Replace this with your actual camera position
			//XMVECTOR cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Replace this with your actual camera target
			//XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Replace this with your actual camera up vector








			//context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
			//context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
			//context->RSSetState(m_states->CullClockwise());



			////m_effect->Apply(context);
			////context->IASetInputLayout(m_inputLayout);

			////primitiveBatch->Begin();
			//////VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
			//////VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
			//////VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);
			//////primitiveBatch->DrawTriangle(v1, v2, v3);

			////VertexPositionColor v1(EntityList[i].position + EntityList[i].size, Colors::Yellow);
			////VertexPositionColor v2(EntityList[i].position + Vector3(-EntityList[i].size.x, EntityList[i].size.y, EntityList[i].size.z), Colors::Yellow);
			////VertexPositionColor v3(EntityList[i].position - EntityList[i].size, Colors::Yellow);
			////VertexPositionColor v4(EntityList[i].position - Vector3(-EntityList[i].size.x, EntityList[i].size.y, EntityList[i].size.z), Colors::Yellow);
			////primitiveBatch->DrawTriangle(v1, v2, v3);
			//////primitiveBatch->DrawQuad(v1, v2, v3, v4);
			////primitiveBatch->End();



			//LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, cubeVerts, cubeIndices));

			//GeometricPrimitive::CreateBox(context, Vector3(1, 1, 1), true, false);

	}
}