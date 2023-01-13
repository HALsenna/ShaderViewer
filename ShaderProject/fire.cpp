#include "fire.h"
#include "ShaderList.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "TextureFactory.h"

using namespace DirectX;

struct Light
{
	XMFLOAT3 lightColor;		// ライトの色
	XMFLOAT3 lightDir;			// ライトの方向
};

// モデルのファイル名
static const char* fileName = "Assets/Model/model_fire.fbx";
static const char* spritefile = "Assets/Model/model_screen.fbx";
Cfire::Cfire(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.3f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_FIRE));
	pModel->m_vertex.pos = pos;
	m_pos = pos;
}

Cfire::~Cfire()
{
}

void Cfire::Init()
{
	// 強い光を生成する
	Light light;
	light.lightColor = XMFLOAT3(5.8f, 5.8f, 5.8f);
	light.lightDir = XMFLOAT3(0.0f, 0.0f, -1.0f);
}

void Cfire::Draw()
{
	int i = 0;
}

void Cfire::SetSprite(Model* pModel)
{
	pModel->Load(spritefile, 1.0f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = {0.0f, 2.0f, 0.0f};
}