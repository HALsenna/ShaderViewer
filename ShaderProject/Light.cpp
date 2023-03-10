#include "Light.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

CLight::CLight()
{
	XMFLOAT3 initPos = { -999.0f, -999.0f, -999.0f };		// 座標
	XMFLOAT3 initDir = { -1.0f, -1.0f, -1.0f };		// 方向
	XMFLOAT3 initColor = { 0.0f, 0.0f, 0.0f };				// 色
	m_DirLight = { initPos, initDir, initColor };			// 太陽光初期化
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

// 太陽光の色の設定
void CLight::SetSkyColor(XMFLOAT3 color)
{
	m_DirLight.color = color;
}

// 太陽光の色情報の取得
XMFLOAT3 CLight::GetSkyColor()
{
	return m_DirLight.color;
}

// 太陽光の方向の取得
void CLight::SetSkyLightDir(XMFLOAT3 direction)
{
	m_DirLight.direction = direction;
}

// 太陽光の方向の取得
XMFLOAT3 CLight::GetSkyLightDir()
{
	return m_DirLight.direction;
}
