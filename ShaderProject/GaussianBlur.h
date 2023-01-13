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
	Texture* m_xRenderTarget;		// �c�u���[�p�����_�[�^�[�Q�b�g
	Texture* m_yRenderTarget;		// ���u���[�p�����_�[�^�[�Q�b�g
	CGeometory* m_pxBlurSprite;		// �c�u���[�p�X�v���C�g
	CGeometory* m_pyBlurSprite;		// ���u���[�p�X�v���C�g
};
