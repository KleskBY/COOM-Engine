#include "pch.h"
#include "OBJLoader.h"

bool LoadOBJ(const std::string& filename, std::vector<std::string>& textureNames,
    std::map<std::string, GeometricPrimitive::VertexCollection>& vertexDataMap,
    std::map<std::string, GeometricPrimitive::IndexCollection>& indexDataMap, bool flip_faces)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open the OBJ file: " << filename << std::endl;
        return false;
    }

    std::string currentTextureName = "default"; // Default texture name in case the OBJ file does not specify texture coordinates

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            XMFLOAT3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (type == "vt")
        {
            XMFLOAT2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if (type == "vn")
        {
            XMFLOAT3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "usemtl")
        {
            iss >> currentTextureName;
            if (std::find(textureNames.begin(), textureNames.end(), currentTextureName) == textureNames.end())
            {
                textureNames.push_back(currentTextureName);
            }
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; ++i)
            {
                int indexPosition, indexTexCoord, indexNormal;
                char separator;
                iss >> indexPosition >> separator >> indexTexCoord >> separator >> indexNormal;

                VertexData vertexData;
                vertexData.position = positions[indexPosition - 1];
                vertexData.texcoord = texcoords[indexTexCoord - 1];
                vertexData.normal = normals[indexNormal - 1];

                // Check if the material already exists in the map
                if (vertexDataMap.find(currentTextureName) == vertexDataMap.end())
                {
                    // If not, create a new entry in the map
                    vertexDataMap[currentTextureName] = GeometricPrimitive::VertexCollection();
                    indexDataMap[currentTextureName] = GeometricPrimitive::IndexCollection();
                }

                auto& vertices = vertexDataMap[currentTextureName];
                auto& indices = indexDataMap[currentTextureName];

                // Check if the vertex already exists in the vertices array
                int index = -1;
                for (size_t j = 0; j < vertices.size(); ++j)
                {
                    if (Vector3(vertices[j].position) == Vector3(vertexData.position) &&
                        Vector3(vertices[j].normal) == Vector3(vertexData.normal) &&
                        Vector2(vertices[j].textureCoordinate) == Vector2(vertexData.texcoord))
                    {
                        index = static_cast<int>(j);
                        break;
                    }
                }

                // If the vertex does not exist, add it to the vertices array
                if (index == -1)
                {
                    vertices.push_back(GeometricPrimitive::VertexType(vertexData.position, vertexData.normal, vertexData.texcoord));
                    index = static_cast<int>(vertices.size() - 1);
                }

                // Add the index to the indices array
                indices.push_back(index);
            }
        }
    }

    file.close();

    if (flip_faces)
    {
        for (auto& indexDataPair : indexDataMap)
        {
            std::reverse(indexDataPair.second.begin(), indexDataPair.second.end());
        }
    }
    return true;
}


// Function to parse an OBJ file and extract vertex and index data separated by texture name
void LoadOBJ(const std::string& filename, std::vector<std::string>& textureNames,
    std::vector<GeometricPrimitive::VertexCollection>& vertexDataArray, std::vector<GeometricPrimitive::IndexCollection>& indexDataArray, bool flip_faces)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open the OBJ file: " << filename << std::endl;
        return;
    }

    std::string currentTextureName = "default"; // Default texture name in case the OBJ file does not specify texture coordinates

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            XMFLOAT3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (type == "vt")
        {
            XMFLOAT2 texcoord;
            iss >> texcoord.x >> texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if (type == "vn")
        {
            XMFLOAT3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "usemtl")
        {
            iss >> currentTextureName;
            std::cout << currentTextureName << vertexDataArray.size() << std::endl;

            if (std::find(textureNames.begin(), textureNames.end(), currentTextureName) == textureNames.end())
            {
                textureNames.push_back(currentTextureName);
                vertexDataArray.push_back(GeometricPrimitive::VertexCollection());
                indexDataArray.push_back(GeometricPrimitive::IndexCollection());
            }
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; ++i)
            {
                int indexPosition, indexTexCoord, indexNormal;
                char separator;
                iss >> indexPosition >> separator >> indexTexCoord >> separator >> indexNormal;

                VertexData vertexData;
                vertexData.position = positions[indexPosition - 1];
                vertexData.texcoord = texcoords[indexTexCoord - 1];
                vertexData.normal = normals[indexNormal - 1];

                // Check if the vertex already exists in the vertices array
                int index = -1;
                auto& vertices = vertexDataArray.back();
                for (size_t j = 0; j < vertices.size(); ++j)
                {
                    if (Vector3(vertices[j].position) == Vector3(vertexData.position) && Vector3(vertices[j].normal) == Vector3(vertexData.normal) && Vector2(vertices[j].textureCoordinate) == Vector2(vertexData.texcoord))
                    {
                        index = static_cast<int>(j);
                        break;
                    }
                }

                // If the vertex does not exist, add it to the vertices array
                if (index == -1)
                {
                    vertices.push_back(GeometricPrimitive::VertexType(vertexData.position, vertexData.normal, vertexData.texcoord));
                    index = static_cast<int>(vertices.size() - 1);
                }

                // Add the index to the indices array
                indexDataArray.back().push_back(index);
            }
        }
    }

    file.close();

    if (flip_faces)
    {
        for (int i = 0; i < indexDataArray.size(); i++)
        {
            std::reverse(indexDataArray[i].begin(), indexDataArray[i].end());
        }
    }
}
