#pragma once
#include "Model.h"
#include "ConstantBuffer.h"

using namespace DirectX;

class PointLight
{
public:
	PointLight(Model* pModel, XMFLOAT3 pos);
	~PointLight();
	void Init();
	void Uninit();
	void Update();
private:
	Model* m_pModel;
public:
	ConstantBuffer* m_lightCB;
	XMFLOAT3 m_pos;
	float m_range;		// ‰e‹¿”ÍˆÍ
	//float m_power;		// ƒ‰ƒCƒg‚Ì‹­‚³
	XMFLOAT4 m_color;
};
