#include "Sprite.h"

CSprite::CSprite()
{

}
CSprite::~CSprite()
{
}
Model* CSprite::Init()
{
	Model* pModel;
	pModel = CreateObj<Model>("TeapotModel");
	pModel->Load("Assets/Model/model_screen.fbx", 1.0f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 0.0f, 2.0f, 0.0f };

	return pModel;
}