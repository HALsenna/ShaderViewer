#include "PointLight.h"
#include "ShaderList.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

using namespace DirectX;
static const char* fileName = "Assets/Model/model_light.fbx";
float lightSet = 0.0f;
//float lightPower = 0.0f;
PointLight::PointLight(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.05f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_POINTLIGHT));
	pModel->m_vertex.pos = pos;
	m_pos = pos;
	m_color = { 5.0f, 0.0f, 0.0f, 1.0f };
	m_range = 1.0f;
	//m_power = 1.0f;
	Init();
}
PointLight::~PointLight()
{

}

void PointLight::Init()
{
	lightSet = 1.0f;
	//lightPower = 1.0f;
}

void PointLight::Update()
{
	if (IsKeyPress(VK_UP))
	{
		m_pos.y += 0.01f;
	}
	else if (IsKeyPress(VK_DOWN))
	{
		m_pos.y -= 0.01f;
	}
	else if (IsKeyPress(VK_RIGHT))
	{
		m_pos.x += 0.01f;
	}
	else if (IsKeyPress(VK_LEFT))
	{
		m_pos.x -= 0.01f;
	}
	m_pModel->m_vertex.pos = m_pos;

	ImGui::SliderFloat("range", &lightSet, 0.0f, 20.0f);
	m_range = lightSet;

	/*ImGui::SliderFloat("power", &lightPower, 1.0f, 100.0f);
	m_power = lightPower;*/
	//ImGui::Text("PointLightPos:%.2f, %.2f, %.2f", m_pos.x, m_pos.y, m_pos.z);
}
