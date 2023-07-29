#include "pch.h"
#include "Viewmodel.h"
#include "render.h"
#include "main.h"
#include <WICTextureLoader.h>
#include "PlayerController.h"

namespace viewmodel
{

    void CreateViewmodelTexture(ID3D11Device* device, std::string textureName)
    {
        std::wstring texture_path(textureName.begin(), textureName.end());
        texture_path = L"data\\sprites\\weapons\\" + texture_path + L".png";
        //DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"cat.png", nullptr, ViewmodelTexture.ReleaseAndGetAddressOf()));


        ComPtr<ID3D11Resource> resource;
        if (CreateWICTextureFromFile(device, texture_path.c_str(), resource.GetAddressOf(), ViewmodelTexture.ReleaseAndGetAddressOf()) != S_OK)
        {
            printf("ERROR LOADING VIEWMODEL: %s", textureName.c_str());
            return;
        }

        ComPtr<ID3D11Texture2D> cat;
        DX::ThrowIfFailed(resource.As(&cat));

        CD3D11_TEXTURE2D_DESC catDesc;
        cat->GetDesc(&catDesc);

        ViewmodelOrigin.x = float(catDesc.Width / 2);
        ViewmodelOrigin.y = float(catDesc.Height / 2);

        //if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, ViewmodelTexture.ReleaseAndGetAddressOf()) != S_OK)
        //{
        //    std::wcout << L"ERROR LOADING TEXTURE: " << texture_path << std::endl;
        //    GenerateTexture(device, texture.ReleaseAndGetAddressOf());
        //}
    }

    void WeaponBob(Vector2 center, float scale)
    {
        Vector3 vel = player::PlayerVelocity;
        vel.y = 0.0f;
        if (vel.Length() > 0.02f)
        {
            float hBobValue = sin(Time * BobSpeed / 2) * scale;
            ViewmodelPos.x = ViewmodelPos.x + hBobValue;

            float vBobValue = ((sin(Time * BobSpeed) * 0.5f) + 0.5f) * scale;
            ViewmodelPos.y = ViewmodelPos.y + vBobValue;
        }
        ViewmodelPos = ViewmodelPos.Lerp(ViewmodelPos, center, BobSpeed * DeltaTime);
    }

    void RenderViewmodel(ID3D11Device* device)
    {
        if (!ViewmodelTexture)
        {
            CreateViewmodelTexture(device, "v_shot");
            return;
        }

        float scale = WindowWidth * 0.0025f;
        Vector2 center = Vector2(WindowWidth / 2.0f, WindowHeight - 50.0f * scale);
        if (ViewmodelPos == Vector2::Zero) ViewmodelPos = center;

        WeaponBob(center, scale);

        m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
        m_spriteBatch->Draw(ViewmodelTexture.Get(), ViewmodelPos, nullptr, Colors::White, ViewmodelAngle, ViewmodelOrigin, scale);
        m_spriteBatch->End();
    }
}