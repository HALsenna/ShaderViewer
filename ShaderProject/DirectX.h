#ifndef __DIRECTX_H__
#define __DIRECTX_H__

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)

ID3D11Device* GetDevice();
ID3D11DeviceContext* GetContext();
IDXGISwapChain* GetSwapChain();

HRESULT InitDX(HWND hWnd, UINT width, UINT height, bool fullscreen);
void UninitDX();
void SwapDX();

void SetCullingMode(D3D11_CULL_MODE cull);
void SetFillMode(D3D11_FILL_MODE fill);

#endif