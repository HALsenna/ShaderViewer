#pragma once
#include "Texture.h"
#include "RenderTarget.h"
#include "Geometory.h"

class GaussianBlur
{
public:
	GaussianBlur();
	~GaussianBlur();
	void Init(Texture* tex);
	void Update();
	void Draw();
	void Uninit();
private:
	void InitRenderTargets();
	void InitSprites();
private:
	Texture* m_texture;
	Texture* m_xRenderTarget;		// 縦ブラー用レンダーターゲット
	Texture* m_yRenderTarget;		// 横ブラー用レンダーターゲット
	CGeometory* m_pxBlurSprite;		// 縦ブラー用スプライト
	CGeometory* m_pyBlurSprite;		// 横ブラー用スプライト
};
