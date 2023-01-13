#include "Chair.h"
#include "ShaderList.h"

// ���f���̃t�@�C����
static const char* fileName = "Assets/Model/model_chair.fbx";
CChair::CChair(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.2f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = pos;
}

CChair::~CChair()
{
}

void CChair::Init()
{

}