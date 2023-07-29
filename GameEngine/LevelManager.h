#pragma once

inline std::vector<std::unique_ptr<DirectX::GeometricPrimitive>> LevelObjects;
inline std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> LevelTextures;
inline std::vector<BoundingBox> LevelBBoxes;
inline std::unique_ptr<DirectX::Model> LevelModel;

void LoadLevel(ID3D11Device* device, ID3D11DeviceContext* context, std::string mapName);
void RenderLevel(ID3D11DeviceContext* context);
inline std::string CurrentLevel;
