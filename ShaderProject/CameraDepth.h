#pragma once
#include "GaussianBlur.h"
#include "Model.h"
class CameraDepth
{
public:
	CameraDepth(Model* sprite);
	~CameraDepth();

	void Init(RenderTarget* defRTV);
	void Update();
	void Draw();

private:
	GaussianBlur* m_gaussian;
	Model* m_pSprite;
};
