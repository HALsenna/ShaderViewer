#pragma once
#include "Model.h"

using namespace DirectX;
class Ctent : public Model
{
public:
	Ctent(Model* pModel, XMFLOAT3 pos);
	~Ctent();
	void Init();
	void Update();
	void Draw();

private:
	XMFLOAT3 m_pos;
	Model* m_pModel;

};
