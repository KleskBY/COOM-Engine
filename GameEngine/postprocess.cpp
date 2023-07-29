#include "pch.h"
#include "postprocess.h"
#include "main.h"

namespace postprocess
{

	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain)
	{
		postProcess = std::make_unique<BasicPostProcess>(pDevice);
	}


	void Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain)
	{
		ID3D11Texture2D* backBuffer = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
		{
			if (backBuffer)
			{
				D3D11_TEXTURE2D_DESC desc;
				backBuffer->GetDesc(&desc);
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

				ID3D11Texture2D* pTexture = NULL;
				if (FAILED(pDevice->CreateTexture2D(&desc, NULL, &pTexture))) return;
				if (!pTexture) return;
				pContext->CopyResource(pTexture, backBuffer);

				D3D11_SHADER_RESOURCE_VIEW_DESC srdes;
				memset(&srdes, 0, sizeof(srdes));
				srdes.Format = desc.Format;
				srdes.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srdes.Texture2D.MostDetailedMip = 0;
				srdes.Texture2D.MipLevels = 1;

				ID3D11ShaderResourceView* postProcessView = NULL;
				if (FAILED(pDevice->CreateShaderResourceView(pTexture, &srdes, &postProcessView))) return;
				pTexture->Release();
				if (postProcessView)
				{
					postProcess->SetSourceTexture(postProcessView);
					postProcess->SetEffect(BasicPostProcess::BloomBlur);
					postProcess->SetBloomBlurParameters(true, 15.f, 3.f);
					postProcess->Process(pContext);
				}
				postProcessView->Release();
				backBuffer->Release();
			}
		}
	}

}