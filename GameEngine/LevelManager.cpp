#include "pch.h"
#include "LevelManager.h"


#include "OBJLoader.h"
#include "e1m1_collision.h"
#include "dx_tools.h"
#include "render.h"

#include "EntityList.h"



void LoadLevel(ID3D11Device* device, ID3D11DeviceContext* context, std::string mapName)
{
    //std::wstring cmo_model(mapName.begin(), mapName.end());
    //cmo_model = cmo_model + L".cmo";
    //LevelModel = Model::CreateFromCMO(device, cmo_model.c_str(), *m_fxFactory);
    CurrentLevel = mapName;

    // Load the .OBJ file
    std::vector<std::string> textureNames;
    std::map<std::string, GeometricPrimitive::VertexCollection> vertexDataMap;
    std::map<std::string, GeometricPrimitive::IndexCollection> indexDataMap;
    std::string objFilename = mapName + ".obj";
    if (!LoadOBJ(objFilename, textureNames, vertexDataMap, indexDataMap, true))
    {
        objFilename = "data/levels/" + mapName + ".obj";
        LoadOBJ(objFilename, textureNames, vertexDataMap, indexDataMap, true);
    }

    for (int i = 0; i < vertexDataMap.size(); i++)
    {
        //We got a MONSTER
        if (textureNames[i].find("monster_") != std::string::npos)
        {
            std::cout << textureNames[i] << std::endl;
            Vector3 pos = vertexDataMap.at(textureNames[i])[0].position;
            AddMonster(textureNames[i], pos);
            for (int j = 0; j < vertexDataMap.at(textureNames[i]).size(); j++)
            {
                Vector3 newPos = vertexDataMap.at(textureNames[i])[j].position;
                if (Vector3::Distance(pos, newPos) > 2.f)
                {
                    pos = newPos;
                    AddMonster(textureNames[i], newPos);
                }
            }
            continue;
        }

        //Creating textures
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        std::wstring texture_path(textureNames[i].begin(), textureNames[i].end());
        texture_path = texture_path + L".dds";
        if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, texture.ReleaseAndGetAddressOf()) != S_OK)
        {
            texture_path = PATH_TEXTURES_TILES + texture_path;
            if (CreateDDSTextureFromFile(device, texture_path.c_str(), nullptr, texture.ReleaseAndGetAddressOf()) != S_OK)
            {
                std::wcout << L"ERROR LOADING TEXTURE: " << texture_path << std::endl;
                GenerateTexture(device, texture.ReleaseAndGetAddressOf());
            }
        }
        LevelTextures.push_back(texture);

        //Creating mesh
        LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, vertexDataMap.at(textureNames[i]), indexDataMap.at(textureNames[i])));
    }

    LoadCollision_E1M1(); //Load collision;
}

void RenderLevel(ID3D11DeviceContext* context)
{
    if (LevelModel)
    {
        LevelModel->Draw(context, *m_states, m_world, m_view, m_proj);
    }

    for (int i = 0; i < LevelObjects.size(); i++)
    {
        //context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
        //context->OMSetDepthStencilState(m_states->DepthNone(), 0);
        //context->RSSetState(m_states->CullNone());


        m_effect->Apply(context);
        context->IASetInputLayout(m_inputLayout);
        m_effect->SetWorld(m_world);
        m_effect->SetView(m_view);
        m_effect->SetProjection(m_proj);
        m_effect->SetTexture(LevelTextures[i].Get());
        LevelObjects[i]->Draw(m_effect.get(), m_inputLayout);

        //LevelObjects[i]->Draw(Matrix::Identity, m_view, m_proj, Colors::White, LevelTextures[i].Get());
    }
}