#include "bonfire.h"
#include "ShaderList.h"
#include "Texture.h"

// モデルのファイル名
static const char* fileName = "Assets/Model/model_bonfire.fbx";
CBonfire::CBonfire(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.1f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = pos;
}

CBonfire::~CBonfire()
{

}

void CBonfire::Init()
{

}