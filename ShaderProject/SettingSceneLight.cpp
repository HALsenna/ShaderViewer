#include "SettingSceneLight.h"


// �V�[���̊���
struct SceneLight
{
	DirectX::XMFLOAT3 dir;		// ���W
	DirectX::XMFLOAT4 color;	// �F
};

// ���C�g�̕���
const DirectX::XMFLOAT3 g_LightDir[LIGHT_MAX] = {
	{ -1.0f, -1.0f, 1.0f },		// �ʏ��ԁ����ォ�烉�C�g�����Ă�
	{ -1.0f, -1.0f, 1.0f },		// �����������ォ�烉�C�g�����Ă�
	{ -1.0f, -1.0f, 1.0f },		// �遨�E�����烉�C�g�����Ă�
};

// ���C�g�̐F
const DirectX::XMFLOAT4 g_LightColor[LIGHT_MAX] = {
	{ 0.1f, 0.1f, 0.0f, 1.0f },		// �ʏ��ԁ��������F���ۂ�����
	{ 0.9f, 0.5f, 0.0f, 1.0f },		// ���������I�����W�F�ɂ���
	{ 0.46f, 0.53f, 0.6f, 1.0f },	// �遨���ۂ����Â��悤�ɂ���
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
	m_currentKind = LIGHT_NORMAL;			// �n�߂͒ʏ��Ԃ�\������
	m_dir = g_LightDir[LIGHT_NORMAL];		// ����
	m_color = g_LightColor[LIGHT_NORMAL];	// �F
}

void CSelectSceneLight::Update()
{

}

// ������n��
DirectX::XMFLOAT3 CSelectSceneLight::GetDir()
{
	return m_dir;
}

// �F��n��
DirectX::XMFLOAT4 CSelectSceneLight::GetColor()
{
	return m_color;
}

// ���C�g��ύX����
void CSelectSceneLight::ChangeLight(KIND_LIGHT_SELECTSCENE kind)
{
	switch (kind)
	{
		// �ʏ��ԁ��������F���ۂ��������烉�C�g�����Ă�
		case LIGHT_NORMAL:
			m_dir = g_LightDir[LIGHT_NORMAL];		// ����
			m_color = g_LightColor[LIGHT_NORMAL];	// �F
			break;
		// ���������I�����W�F�ō������烉�C�g�����Ă�
		case LIGHT_INSIDE:
			m_dir = g_LightDir[LIGHT_INSIDE];		// ����
			m_color = g_LightColor[LIGHT_INSIDE];	// �F
			break;
		// �遨�Â��E�����烉�C�g�����Ă�
		case LIGHT_NIGHT:
			m_dir = g_LightDir[LIGHT_NIGHT];		// ����
			m_color = g_LightColor[LIGHT_NIGHT];	// �F
			break;
	}
}
