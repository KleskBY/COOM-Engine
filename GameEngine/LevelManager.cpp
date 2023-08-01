#include "pch.h"
#include "LevelManager.h"
#include "OBJLoader.h"
#include "dx_tools.h"
#include "render.h"

#include "EntityList.h"
#include "PlayerController.h"

void extractBoundingBoxes(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::vector<Vector3> vertices;
    std::string line;
    std::string prefix;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        iss >> prefix;

        if (prefix == "v") 
        {
            Vector3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }

        // For simplicity, we consider each new object in the .OBJ file as a separate bounding box.
        if (prefix == "o") 
        {
            std::string objectName;
            iss >> objectName;

            if (objectName.find("monster_") != std::string::npos || objectName.find("info_") != std::string::npos) //ent
            {
                vertices.clear();
            }
            else
            {
                if (!vertices.empty())
                {
                    Vector3 minPoint, maxPoint;
                    minPoint = maxPoint = vertices[0];
                    for (const auto& vertex : vertices)
                    {
                        if (vertex.x < minPoint.x) minPoint.x = vertex.x;
                        if (vertex.y < minPoint.y) minPoint.y = vertex.y;
                        if (vertex.z < minPoint.z) minPoint.z = vertex.z;

                        if (vertex.x > maxPoint.x) maxPoint.x = vertex.x;
                        if (vertex.y > maxPoint.y) maxPoint.y = vertex.y;
                        if (vertex.z > maxPoint.z) maxPoint.z = vertex.z;
                    }
                    BoundingBox box;
                    box.Extents = (maxPoint - minPoint) / 2;
                    box.Center = (minPoint + maxPoint) / 2;
                    LevelBBoxes.push_back(box);
                }
                vertices.clear();
            }
        }
    }

    file.close();
}

void UnloadLevel()
{
    CurrentLevel = "";
    LevelBBoxes.clear();

    for (int i = 0; i < LevelTextures.size(); i++)
    {
        LevelTextures[i].ReleaseAndGetAddressOf();
    }
    LevelTextures.clear();

    for (int i = 0; i < LevelObjects.size(); i++)
    {
        LevelObjects[i].release();
    }
    LevelObjects.clear();


    for (int i = 0; i < EntityList.size(); i++)
    {
        EntityList[i].think = SUB_Remove;
        EntityList[i].next_think = 0;
    }
}

void LoadLevel(ID3D11Device* device, ID3D11DeviceContext* context, std::string mapName)
{
    UnloadLevel();
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
    extractBoundingBoxes(objFilename);

    for (int i = 0; i < vertexDataMap.size(); i++)
    {
        //We got a MONSTER
        if (textureNames[i].find("monster_") != std::string::npos)
        {
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
        if (textureNames[i].find("info_player_start") != std::string::npos) //Start point
        {
            Vector3 pos = vertexDataMap.at(textureNames[i])[0].position;
            player::PlayerPosition = pos;
            player::PlayerVelocity = Vector3::Zero;
            player::PlayerGrounded = true;
            player::PlayerHealth = 100;
            continue;
        }

        //Creating textures
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        std::string texture_path = textureNames[i];
        texture_path = texture_path + ".dds";
        if (CreateDDSTextureFromFile(device, STR2WSTR(texture_path).c_str(), nullptr, texture.ReleaseAndGetAddressOf()) != S_OK)
        {
            texture_path = PATH_TEXTURES_TILES + texture_path;
            if (CreateDDSTextureFromFile(device, STR2WSTR(texture_path).c_str(), nullptr, texture.ReleaseAndGetAddressOf()) != S_OK)
            {
                std::cout << "ERROR LOADING TEXTURE: " << texture_path << std::endl;
                GenerateTexture(device, texture.ReleaseAndGetAddressOf());
            }
        }
        LevelTextures.push_back(texture);

        //Creating mesh
        LevelObjects.push_back(GeometricPrimitive::CreateCustom(context, vertexDataMap.at(textureNames[i]), indexDataMap.at(textureNames[i])));
    }
}

void RenderLevel(ID3D11DeviceContext* context)
{
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