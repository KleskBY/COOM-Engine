#pragma once
#include "main.h"

void DrawRay(ID3D11DeviceContext* pContext, FXMVECTOR Origin, FXMVECTOR Direction, bool bNormalize, FXMVECTOR color);
void DrawAabb(ID3D11DeviceContext* pContext, const BoundingBox& box, FXMVECTOR color);

void Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain);