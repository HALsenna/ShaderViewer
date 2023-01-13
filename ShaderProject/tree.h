#pragma once
#include "Model.h"
#include "SceneBase.hpp"

using namespace DirectX;
class CTree : public Model
{
public:
	CTree(Model* pModel, XMFLOAT3 pos);
	~CTree();
	void Init();
	void Update();
	void Draw();
	void SetSprite(Model* pModel);

private:
	XMFLOAT3 m_pos;
	Model* m_pModel;

};
