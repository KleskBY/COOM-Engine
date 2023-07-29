#pragma once

inline std::unique_ptr<DirectX::GeometricPrimitive> Sky;
inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SkyboxTexture;
constexpr float SKY_SIZE = 200.0f;


void RenderSky(ID3D11DeviceContext* context);
void CreateSky(ID3D11Device* device, ID3D11DeviceContext* context, std::string textureName);