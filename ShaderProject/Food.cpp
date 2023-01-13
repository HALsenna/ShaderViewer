#include "Food.h"
#include "ShaderList.h"
namespace
{
	const char* pizzaModel = "Assets/Model/Food/model_pizza.fbx";
	//const char* pizzaModel = "Assets/Model/Player/model_player_01.fbx";
}
CFood::CFood()
{
	//Init();
}

CFood::~CFood()
{
}

Model* CFood::Init()
{
	Model* pModel;
	pModel = new Model();
	pModel->Load(pizzaModel, 0.05f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pModel->m_vertex.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_foodModels[KIND_PIZZA] = pModel;

	return pModel;
}

void CFood::SetFood(KIND kind)
{
}

void CFood::Draw(ConstantBuffer* cb, XMFLOAT4X4 mat[3])
{
	XMMATRIX worldMatrix = XMMatrixIdentity();

	// ‰ñ“]
	worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_foodModels[KIND_PIZZA]->m_vertex.rot.x));
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_foodModels[KIND_PIZZA]->m_vertex.rot.y));
	worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_foodModels[KIND_PIZZA]->m_vertex.rot.z));

	// À•W•ÏŠ·
	worldMatrix *= XMMatrixTranslation(m_foodModels[KIND_PIZZA]->m_vertex.pos.x, m_foodModels[KIND_PIZZA]->m_vertex.pos.y, m_foodModels[KIND_PIZZA]->m_vertex.pos.z);
	//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));

	cb->Write(&mat);
	cb->BindVS(0);

	m_foodModels[KIND_PIZZA]->Draw();
}


