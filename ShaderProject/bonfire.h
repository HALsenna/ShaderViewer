#pragma once
#include "Model.h"

using namespace DirectX;
class CBonfire : public Model
{
public:
	CBonfire(Model* pModel, XMFLOAT3 pos);
	~CBonfire();
	void Init();
	void Update();
	void Draw();

private:
	XMFLOAT3 m_pos;
	Model* m_pModel;

};
