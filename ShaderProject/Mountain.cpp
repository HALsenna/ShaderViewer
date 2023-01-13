#include "Mountain.h"
#include "ShaderList.h"

// モデルのファイル名
static const char* fileName = "Assets/Model/model_ground.fbx";
CMountain::CMountain(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 1.0f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = pos;
}

CMountain::~CMountain()
{
}

void CMountain::Init()
{

}