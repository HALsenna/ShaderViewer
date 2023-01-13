#pragma once
#include "Model.h"

using namespace DirectX;
class CMountain : public Model
{
public:
	CMountain(Model* pModel, XMFLOAT3 pos);
	~CMountain();
	void Init();
	void Update();
	void Draw();

private:
	Model* m_pModel;

};
