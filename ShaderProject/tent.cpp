#include "tent.h"
#include "ShaderList.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "TextureFactory.h"

class SceneBase;
using namespace DirectX;

struct Light
{
	XMFLOAT3 lightColor;		// ライトの色
	XMFLOAT3 lightDir;			// ライトの方向
};

// モデルのファイル名
static const char* fileName = "Assets/Model/model_tent_02.fbx";
Ctent::Ctent(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.005f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = pos;
}

Ctent::~Ctent()
{
}

void Ctent::Init()
{
}