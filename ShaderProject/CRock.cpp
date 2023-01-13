#include "CRock.h"
#include "ShaderList.h"
#include "Texture.h"

// モデルのファイル名
static const char* fileName = "Assets/Model/model_rock.FBX";
CRock::CRock(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.3f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_ROCK));
	pModel->m_vertex.pos = pos;
}

CRock::~CRock()
{
}


void CRock::Init()
{

}