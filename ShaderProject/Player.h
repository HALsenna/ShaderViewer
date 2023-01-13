#pragma once
#include "Model.h"
#include "CameraDCC.h"

using namespace DirectX;
class CPlayer : public Model
{
public:
	CPlayer(Model* pModel, XMFLOAT3 pos);
	~CPlayer();
	void Init();
	void Update(XMFLOAT3 healPos);
	void Draw();
	void Heal();

private:
	Model* m_pModel;
	float m_healPointRange;

public:
	CameraDCC* m_camera;
	bool canMove;
	bool m_isHealPoint = false;
};
