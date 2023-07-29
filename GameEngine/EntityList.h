#pragma once


typedef void (*TH_FUNC)();

struct ENTITY
{
	std::string name;
	Vector3 position;
	Vector3 angles;
	Vector3 size;
	Vector3 velocity;
	int health;
	std::string model;
	BoundingBox bbox;
	bool sprite;

	float next_think;
	TH_FUNC think = nullptr;
};



inline std::vector<ENTITY> EntityList;
//inline std::vector<std::unique_ptr<DirectX::GeometricPrimitive>> ModelPool;
inline std::unique_ptr<DirectX::GeometricPrimitive> ModelPool[1024];
inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TexturePool[1024];
inline ENTITY* self;
inline int selfID;

void RenderEntityList(ID3D11Device* device, ID3D11DeviceContext* context);
void AddMonster(std::string name, Vector3 pos);
void SUB_Remove();