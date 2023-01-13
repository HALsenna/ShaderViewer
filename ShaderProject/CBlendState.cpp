#include "CBlendState.h"
#include "DirectX.h"

static ID3D11Device* g_pDevice;							// デバイス
static ID3D11DeviceContext* g_pDeviceContext;			// デバイスコンテキスト
static ID3D11BlendState* g_pBlendState[MAX_BLENDSTATE];	// ブレンドステート
ID3D11DepthStencilState* g_pDSS[2];						// Zステンシル ステート


void CBlendState::Init()
{
	g_pDevice = GetDevice();
	g_pDeviceContext = GetContext();

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[0]);

	// ブレンドステート生成(アルファブレンド用)
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);

	// ブレンドステート生成(加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);

	// ブレンドステート生成(減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);

	SetBlendState(BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);
}

void CBlendState::SetBlendState(int nState)
{
	g_pDeviceContext = GetContext();
	if (nState >= 0 && nState < MAX_BLENDSTATE)
	{
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pDeviceContext->OMSetBlendState(g_pBlendState[nState], blendFactor, 0xffffffff);
	}
}

// 深度バッファの設定
void CBlendState::SetZBuffer(bool bEnable)
{
	g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1], 0);
}

void CBlendState::Uninit()
{
	// ブレンドステート解放
	for (int i = 0; i < MAX_BLENDSTATE; ++i)
	{
		SAFE_RELEASE(g_pBlendState[i]);
	}

	// 深度ステンシルステート解放
	for (int i = 0; i < _countof(g_pDSS); ++i) {
		SAFE_RELEASE(g_pDSS[i]);
	}

	// デバイスコンテキストの解放
	SAFE_RELEASE(g_pDeviceContext);

	// デバイスの解放
	SAFE_RELEASE(g_pDevice);
}