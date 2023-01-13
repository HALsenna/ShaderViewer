#pragma once
#include "Model.h"
#include "ConstantBuffer.h"
#include "Sound.h"

using namespace DirectX;
class Cmeat : public Model
{
public:
	Cmeat(Model* pModel, XMFLOAT3 pos);
	~Cmeat();
	void Init();
	void Update();
	void Draw();

public:
	enum STATE
	{
		STATE_IDLE = 0,			// �ʏ���
		STATE_GRILLD,			// �Ă���Ă�����
		STATE_STAGING,			// ���o���
		STATE_MAX,
	};

	

	// �]��
	enum EVALUTION
	{
		EVA_RARE = 0,		// ���Ă�
		EVA_GRILLD,			// ���񂪂�
		EVA_BURNT,			// �ł�
		EVA_MAX,
	};

public:
	enum KIND
	{
		KIND_ROW = 0,		// ��
		KIND_RARE,			// ���Ă�
		KIND_GRILLD,		// ���񂪂�
		KIND_BURNT,			// �ł�
		KIND_MAX,
	};

private:
	void ChangeState(STATE state);		// ���o��Ԃ�ύX����
	void UpdateState(STATE state);		// ���o��Ԃ̍X�V����
	EVALUTION CheckMeatKind();				// �����̏Ă����]������

private:
	XMFLOAT3 m_pos;			// ���W
	Model* m_pModel;		// ���f��
	eBGM m_stagingBGM[EVA_MAX];		// ���oBGM
	EVALUTION m_evalution;
public:
	float m_maxCookTime;
	float m_condition = 0.0f;
	KIND m_meatKind = KIND_ROW;		// �����̎��
	STATE m_stagingState = STATE_IDLE;	// ���o�̏��
	ConstantBuffer* m_cb;
	ConstantBuffer* m_specularCB;
	Model* m_pBoneModel;
};
