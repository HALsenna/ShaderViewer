#pragma once
#include "Model.h"
#include "SceneBase.hpp"

using namespace DirectX;
class Cfire : public Model
{
public:
	Cfire(Model* pModel, XMFLOAT3 pos);
	~Cfire();
	void Init();
	void Update();
	void Draw();
	void SetSprite(Model* pModel);

private:
	XMFLOAT3 m_pos;
	Model* m_pModel;

};
