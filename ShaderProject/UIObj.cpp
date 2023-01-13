#include "UIObj.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

CUI::CUI(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, Texture* pTex)
{
	m_uiInfo.pos = pos;
 	m_uiInfo.size = XMFLOAT2(pTex->GetWidth() * size.x, pTex->GetHeight() * size.y);
	m_uiInfo.color = color;
	m_fTime = 0.0f;
	m_dissolveRate = 0.0f;
	m_pTex = pTex;

	// �|���S���̐���
	m_pGeometory = new CGeometory();
	m_pGeometory->InitGeometory();		// �I�u�W�F�N�g�̐���

	m_state = STAGING_NONE;

	Init();
}

CUI::~CUI()
{
	delete m_pGeometory;
}

void CUI::Init()
{

}

void CUI::Update()
{
	// �^�C�}�[�X�V
	m_fTime += 1.0f / 60.0f;

	// ���o
	#pragma region staging
	switch (m_state)
	{
	// �t�F�[�h�C��
	case STAGING_FADEIN:
		// �F���̃Z�b�g
		m_uiInfo.color.w = m_fTime / m_fFadeSec;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;
	
	// �t�F�[�h�A�E�g
	case STAGING_FADEOUT:
		// �F���̃Z�b�g
		m_uiInfo.color.w = 1.0f - m_fTime / m_fFadeSec;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;

	case STAGING_FLASH:
		// �F���̃Z�b�g
		if (m_bFlashFadeIn)
			m_uiInfo.color.w = 1.0f - m_fTime / m_fFlashSpeed;
		else
			m_uiInfo.color.w = m_fTime / m_fFlashSpeed;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFlashSpeed)
		{
			m_fTime = 0.0f;
			m_bFlashFadeIn = !m_bFlashFadeIn;
		}

		break;

	// �f�B�]���u���o
	case STAGING_DISSOLOVE:
		m_dissolveRate = 1.0f - m_fTime / m_dissolveTime;
		m_pGeometory->SetDissolve(m_dissolveRate, m_pDissolveTex->GetResource());
		break;
	case STAGING_NONE:
		break;
	case STAGING_MAX:
		break;
	default:
		break;
	}
	#pragma endregion
}

void CUI::Draw()
{
	m_pGeometory->SetTexture(m_pTex->GetResource());
	m_pGeometory->SetColor(m_uiInfo.color);
	m_pGeometory->DrawPolygon(m_uiInfo.pos, m_uiInfo.size, XMFLOAT3(0.0f, 0.0f, 0.0f));
}

// �t�F�[�h����
void CUI::Fade(STAGINGSTATE state, float fadesec)
{
	m_state = state;
	m_fTime = 0.0f;
	m_fFadeSec = fadesec;
}

// �t���b�V������
void CUI::Flash(float flashSpeed)
{
	m_state = STAGING_FLASH;
	m_bFlashFadeIn = true;
	m_fFlashSpeed = flashSpeed;
}

// �f�B�]���u����
void CUI::Dissolve(float dissolvesec, Texture* pTex)
{
	m_state = STAGING_DISSOLOVE;
	m_dissolveTime = dissolvesec;
	m_pDissolveTex = pTex;
}

void CUI::SetPos(XMFLOAT3 pos)
{
	m_uiInfo.pos = pos;
}

XMFLOAT3 CUI::GetPos()
{
	return m_uiInfo.pos;
}
