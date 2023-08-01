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
	std::shared_ptr<GeometricPrimitive> mesh;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

	float next_think;
	TH_FUNC think = nullptr;
};


inline std::vector<ENTITY> EntityList;
inline ENTITY* self;
inline int selfID;

void RenderEntityList(ID3D11Device* device, ID3D11DeviceContext* context);
void AddMonster(std::string name, Vector3 pos);
void SUB_Remove();