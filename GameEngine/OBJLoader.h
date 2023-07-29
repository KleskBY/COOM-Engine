#pragma once
#include "pch.h"
#include "main.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

struct VertexData
{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

struct MaterialGroup
{
    std::string textureName;
    GeometricPrimitive::VertexCollection vertices;
    GeometricPrimitive::IndexCollection indices;
};

bool LoadOBJ(const std::string& filename, std::vector<std::string>& textureNames,
    std::map<std::string, GeometricPrimitive::VertexCollection>& vertexDataMap,
    std::map<std::string, GeometricPrimitive::IndexCollection>& indexDataMap, bool flip_faces);

// Function to parse an OBJ file and extract vertex and index data separated by texture name
void LoadOBJ(const std::string& filename, std::vector<std::string>& textureNames, std::vector<GeometricPrimitive::VertexCollection>& vertexDataArray, std::vector<GeometricPrimitive::IndexCollection>& indexDataArray, bool flip_faces = true);
