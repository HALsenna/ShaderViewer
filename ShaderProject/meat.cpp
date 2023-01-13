#include "meat.h"
#include "ShaderList.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "TextureFactory.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
using namespace DirectX;

static float specular;

struct Light
{
	XMFLOAT3 lightColor;		// ���C�g�̐F
	XMFLOAT3 lightDir;			// ���C�g�̕���
};

// ���f���̃t�@�C����
static const char* fileName = "Assets/Model/model_meat.fbx";
Cmeat::Cmeat(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.3f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_MEAT));
	pModel->m_vertex.pos = pos;
	//m_maxCookTime = 155.0f;
	m_maxCookTime = 30.0f;
}

Cmeat::~Cmeat()
{
}

void Cmeat::Init()
{
	// ���oBGM���i�[����
	m_stagingBGM[0] = BGM_PERFECT;
}

void Cmeat::Update()
{
	if (IsKeyTrigger('M'))
	{
		switch (m_stagingState)
		{
			// �ҋ@��Ԃ̏ꍇ
			case STATE_IDLE:
				// �Ă���Ԃɂ���
				ChangeState(STATE_GRILLD);
				break;

			// �Ă���Ԃ̏ꍇ
			case STATE_GRILLD:
				// �Ă���]���Ɉڂ�
				ChangeState(STATE_STAGING);
				break;
			// ���o��Ԃ̏ꍇ
			case STATE_STAGING:
				// ���ɉ������Ȃ�
				break;
			default:
				break;

		}
	}
	UpdateState(m_stagingState);
	specular = 1.0f;
	ImGui::SliderFloat("specular", &specular, 1.0f, 100.0f);
}

void Cmeat::ChangeState(STATE state)
{
	switch (state)
	{
	// �ҋ@���
	case Cmeat::STATE_IDLE:
		break;
	// �Ă�
	case Cmeat::STATE_GRILLD:
		CSound::Play(BGM_COOKING);			// ����BGM�̍Đ�
		CSound::SetVolume(BGM_COOKING, 3.0f);
		CSound::Play(BGM_GRILL);			// ����BGM�̍Đ�
		CSound::SetVolume(BGM_GRILL, 0.5f);
		break;
	case Cmeat::STATE_STAGING:
		CSound::Stop(BGM_COOKING);			// ����BGM�̒�~
		CSound::Stop(BGM_GRILL);			// ����BGM�̍Đ�

		// ���ʂɉ�����BGM���Đ�����
		m_evalution = CheckMeatKind();
		if (m_evalution == EVA_GRILLD)
		{
			CSound::Play(BGM_PERFECT);
		}
		break;
	case Cmeat::STATE_MAX:
		break;
	}
	m_stagingState = state;
}

void Cmeat::UpdateState(STATE state)
{
	switch (state)
	{
		// �ҋ@���
		case Cmeat::STATE_IDLE:
			// ���ɉ������Ȃ�
			XMFLOAT4 cb = { specular, 0.0f, 0.0f, 0.0f };
			m_specularCB->Write(&cb);
			m_specularCB->BindPS(3);
		break;
		// �Ă�
		case Cmeat::STATE_GRILLD:
			m_condition += 0.1f;
			ImGui::Text("cooking:%.2f", m_condition);
			m_pModel->m_vertex.rot.x += 0.5f;
			m_pBoneModel->m_vertex.rot.x += 0.5f;
			
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			cb = { m_condition, m_maxCookTime, 0.0f, 0.0f };
			m_cb->Write(&cb);
			m_cb->BindPS(2);
			cb = { specular, 0.0f, 0.0f, 0.0f };
			m_specularCB->Write(&cb);
			m_specularCB->BindPS(3);

			if (m_condition > m_maxCookTime + 30.0f)
				ChangeState(STATE_STAGING);
		break;

		// ���ʉ��o��
		case Cmeat::STATE_STAGING:
			if (!CSound::IsPlaying(BGM_PERFECT))
			{
				CSound::Stop(BGM_PERFECT);
				// �Ă�������Z�b�g����
				m_condition = 0.0f;
				// �p�x�����Z�b�g����
				m_vertex.rot = { 0.0f, 0.0f, 0.0f };
				// ���̐F��ݒ肷��
				cb = { m_condition, m_maxCookTime, 0.0f, 0.0f };
				m_cb->Write(&cb);
				m_cb->BindPS(2);
				cb = { specular, 0.0f, 0.0f, 0.0f };
				m_specularCB->Write(&cb);
				m_specularCB->BindPS(3);
				// �ҋ@��Ԃɖ߂�
				ChangeState(STATE_IDLE);
			}
			
		break;
		case Cmeat::STATE_MAX:
		break;
	}
}

Cmeat::EVALUTION Cmeat::CheckMeatKind()
{
	EVALUTION ev = EVA_MAX;

	// �p�[�t�F�N�g���
	if (m_condition >= m_maxCookTime && m_condition < m_maxCookTime + 5.0f)
	{
		ev = EVA_GRILLD;
	}
	// ���Ă����
	else if (m_condition < m_maxCookTime)
	{
		ev = EVA_RARE;
	}
	// �ł����
	else if (m_condition >= m_maxCookTime + 5.0f)
	{
		ev = EVA_BURNT;
	}

	return ev;
}
