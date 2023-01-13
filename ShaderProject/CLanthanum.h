#pragma once
#include "Model.h"

using namespace DirectX;
class CLanthanum : public Model
{
public:
	CLanthanum(Model* pModel, XMFLOAT3 pos);
	~CLanthanum();
	void Init();
	void Update();
	void Draw();

private:
	XMFLOAT3 m_pos;
	Model* m_pModel;

};
