#pragma once
#include "Model.h"

using namespace DirectX;
class CChair : public Model
{
public:
	CChair(Model* pModel, XMFLOAT3 pos);
	~CChair();
	void Init();
	void Update();
	void Draw();

private:
	Model* m_pModel;

};
