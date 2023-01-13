#include "tree.h"
#include "ShaderList.h"
#include "Texture.h"

// モデルのファイル名
static const char* fileName = "Assets/Model/model_tree.fbx";
CTree::CTree(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.01f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = pos;
}

CTree::~CTree()
{
}

void CTree::Init()
{

}