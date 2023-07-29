#pragma once


namespace viewmodel
{

	inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ViewmodelTexture;
	inline DirectX::SimpleMath::Vector2 ViewmodelPos;
	inline DirectX::SimpleMath::Vector2 ViewmodelOrigin;
	inline float ViewmodelAngle = 0.0f;

	void CreateViewmodelTexture(ID3D11Device* device, std::string textureName);
	void RenderViewmodel(ID3D11Device* device);

	inline float BobSpeed = 10.0f;
}