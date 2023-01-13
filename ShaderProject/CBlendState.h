#pragma once

// �u�����h�X�e�[�g
enum EBlendState
{
	BS_NONE = 0,							// ��������������
	BS_ALPHABLEND,							// ����������
	BS_ADDITIVE,							// ���Z����
	BS_SUBTRACTION,							// ���Z����

	MAX_BLENDSTATE
};
class CBlendState
{
public:
	static void Init();
	static void Uninit();
	static void SetBlendState(int nState);			// �u�����h�X�e�[�g�̐ݒ�
	static void SetZBuffer(bool bEnable);
};
