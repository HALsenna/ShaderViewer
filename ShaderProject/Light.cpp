#include "Light.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

CLight::CLight()
{
	XMFLOAT3 initPos = { -999.0f, -999.0f, -999.0f };		// ���W
	XMFLOAT3 initDir = { -1.0f, -1.0f, -1.0f };		// ����
	XMFLOAT3 initColor = { 0.0f, 0.0f, 0.0f };				// �F
	m_DirLight = { initPos, initDir, initColor };			// ���z��������
}

CLight::~CLight()
{

}

void CLight::Init()
{
}

void CLight::Update()
{
	float setColor[3] = { m_DirLight.color.x, m_DirLight.color.y, m_DirLight.color.z };
	ImGui::SliderFloat3("color", setColor, 0.0f, 1.0f);
	m_DirLight.color = { setColor[0], setColor[1], setColor[2] };

	float setDir[3] = { m_DirLight.direction.x, m_DirLight.direction.y, m_DirLight.direction.z };
	ImGui::SliderFloat3("direction", setDir, -1.0f, 1.0f);
	m_DirLight.direction = { setDir[0], setDir[1], setDir[2] };
}

void CLight::Draw()
{
}

// ���z���̐F�̐ݒ�
void CLight::SetSkyColor(XMFLOAT3 color)
{
	m_DirLight.color = color;
}

// ���z���̐F���̎擾
XMFLOAT3 CLight::GetSkyColor()
{
	return m_DirLight.color;
}

// ���z���̕����̎擾
void CLight::SetSkyLightDir(XMFLOAT3 direction)
{
	m_DirLight.direction = direction;
}

// ���z���̕����̎擾
XMFLOAT3 CLight::GetSkyLightDir()
{
	return m_DirLight.direction;
}
