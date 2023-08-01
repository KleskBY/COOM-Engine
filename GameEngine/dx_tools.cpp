#include "pch.h"
#include "dx_tools.h"


void GenerateTexture(ID3D11Device* pDevice, ID3D11ShaderResourceView** texture)
{
	ID3D11Texture2D* texc = nullptr;
	uint32_t pixelcolor = 0xFFFFFFFF; //0xff00ff00 green, 0xffff0000 blue, 0xff0000ff red, 0x00FFFFFF white, 0xFFFFFFFF  black
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	D3D11_SUBRESOURCE_DATA initData = { &pixelcolor, sizeof(uint32_t), 0 };

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	pDevice->CreateTexture2D(&desc, &initData, &texc);

	if (texc)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srdes;
		memset(&srdes, 0, sizeof(srdes));
		srdes.Format = format;
		srdes.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srdes.Texture2D.MostDetailedMip = 0;
		srdes.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(texc, &srdes, texture);
	}
}



XMMATRIX LookInDirection(const XMVECTOR& position, const XMVECTOR& target, const XMVECTOR& up)
{
	XMVECTOR forward = XMVector3Normalize(target - position);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, forward));
	XMVECTOR newUp = XMVector3Cross(forward, right);

	XMMATRIX rotation(
		right,
		newUp,
		forward,
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
	);

	return rotation;
}









/*
//load png from memory using stb_image. Do not show it to Arting - legendary reverse engineer and Umbrella coder lol


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "mino.h"
void LoadTexture(ID3D11Device* device, ID3D11ShaderResourceView** textureView)
{
	int w, h, channels;
	unsigned char* image_data = stbi_load_from_memory(mino, sizeof(mino), &w, &h, &channels, 4);

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = w;
	desc.Height = h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	device->CreateTexture2D(&desc, &subResource, &pTexture);

	if (pTexture)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		device->CreateShaderResourceView(pTexture, &srvDesc, textureView);
		pTexture->Release();
	}
	free(image_data);
}

*/
