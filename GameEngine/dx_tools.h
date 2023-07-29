#pragma once
#include "main.h"

void GenerateTexture(ID3D11Device* pDevice, ID3D11ShaderResourceView** texture);
XMMATRIX LookInDirection(const XMVECTOR& position, const XMVECTOR& target, const XMVECTOR& up);
