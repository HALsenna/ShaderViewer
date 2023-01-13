#pragma once
#include <DirectXMath.h>

// �����̎��
enum KIND_LIGHT_SELECTSCENE
{
	LIGHT_NORMAL = 0,		//�@�ʏ���
	LIGHT_INSIDE,			// ������
	LIGHT_NIGHT,			// ��
	
	LIGHT_MAX
};

// �Z���N�g�V�[���̊���
class CSelectSceneLight
{
// �����o�[�֐�
public:
	CSelectSceneLight();
	~CSelectSceneLight();
	void Init();
	void Update();
	DirectX::XMFLOAT3 GetDir();
	DirectX::XMFLOAT4 GetColor();
	void ChangeLight(KIND_LIGHT_SELECTSCENE kind);		// ���C�g��ύX����

// �����o�[�ϐ�
private:
	KIND_LIGHT_SELECTSCENE m_currentKind;		// ���ݕ\�����Ă��郉�C�g�̎��
	DirectX::XMFLOAT3 m_dir;		// ���C�g�̕���
	DirectX::XMFLOAT4 m_color;		// ���C�g�̐F

};
