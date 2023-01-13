#include "SettingSceneLight.h"


// シーンの環境光
struct SceneLight
{
	DirectX::XMFLOAT3 dir;		// 座標
	DirectX::XMFLOAT4 color;	// 色
};

// ライトの方向
const DirectX::XMFLOAT3 g_LightDir[LIGHT_MAX] = {
	{ -1.0f, -1.0f, 1.0f },		// 通常状態→左上からライトをあてる
	{ -1.0f, -1.0f, 1.0f },		// 建物内→左上からライトをあてる
	{ -1.0f, -1.0f, 1.0f },		// 夜→右下からライトをあてる
};

// ライトの色
const DirectX::XMFLOAT4 g_LightColor[LIGHT_MAX] = {
	{ 0.1f, 0.1f, 0.0f, 1.0f },		// 通常状態→少し黄色っぽくする
	{ 0.9f, 0.5f, 0.0f, 1.0f },		// 建物内→オレンジ色にする
	{ 0.46f, 0.53f, 0.6f, 1.0f },	// 夜→青っぽく薄暗いようにする
};

CSelectSceneLight::CSelectSceneLight()
{
	Init();
}

CSelectSceneLight::~CSelectSceneLight()
{
}

void CSelectSceneLight::Init()
{
	m_currentKind = LIGHT_NORMAL;			// 始めは通常状態を表示する
	m_dir = g_LightDir[LIGHT_NORMAL];		// 方向
	m_color = g_LightColor[LIGHT_NORMAL];	// 色
}

void CSelectSceneLight::Update()
{

}

// 方向を渡す
DirectX::XMFLOAT3 CSelectSceneLight::GetDir()
{
	return m_dir;
}

// 色を渡す
DirectX::XMFLOAT4 CSelectSceneLight::GetColor()
{
	return m_color;
}

// ライトを変更する
void CSelectSceneLight::ChangeLight(KIND_LIGHT_SELECTSCENE kind)
{
	switch (kind)
	{
		// 通常状態→少し黄色っぽく左側からライトをあてる
		case LIGHT_NORMAL:
			m_dir = g_LightDir[LIGHT_NORMAL];		// 方向
			m_color = g_LightColor[LIGHT_NORMAL];	// 色
			break;
		// 建物内→オレンジ色で左側からライトをあてる
		case LIGHT_INSIDE:
			m_dir = g_LightDir[LIGHT_INSIDE];		// 方向
			m_color = g_LightColor[LIGHT_INSIDE];	// 色
			break;
		// 夜→青暗く右下からライトをあてる
		case LIGHT_NIGHT:
			m_dir = g_LightDir[LIGHT_NIGHT];		// 方向
			m_color = g_LightColor[LIGHT_NIGHT];	// 色
			break;
	}
}
