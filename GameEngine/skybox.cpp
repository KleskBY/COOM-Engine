#include "pch.h"
#include "skybox.h"
#include "main.h"
#include "dx_tools.h"

void CreateSky(ID3D11Device* device, ID3D11DeviceContext* context, std::string textureName)
{
    std::wstring texture_path(textureName.begin(), textureName.end());
    texture_path = texture_path + L".dds";
    if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, SkyboxTexture.ReleaseAndGetAddressOf()) != S_OK)
    {
        texture_path = L"data\\sky\\" + texture_path;
        if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, SkyboxTexture.ReleaseAndGetAddressOf()) != S_OK)
        {
            //std::wcout << L"ERROR LOADING TEXTURE: " << texture_path << std::endl;
            GenerateTexture(device, SkyboxTexture.ReleaseAndGetAddressOf());
        }
    }
    //Sky = GeometricPrimitive::CreateBox(context, XMFLOAT3(10, 10, 10), false, true);
	Sky = GeometricPrimitive::CreateSphere(context, SKY_SIZE, 16U, false, true);
}

void RenderSky(ID3D11DeviceContext* context)
{
    if(Sky && SkyboxTexture) Sky->Draw(Matrix::Identity, m_view, m_proj, Colors::White, SkyboxTexture.Get());

}