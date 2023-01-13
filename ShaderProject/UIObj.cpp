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

	// ポリゴンの生成
	m_pGeometory = new CGeometory();
	m_pGeometory->InitGeometory();		// オブジェクトの生成

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
	// タイマー更新
	m_fTime += 1.0f / 60.0f;

	// 演出
	#pragma region staging
	switch (m_state)
	{
	// フェードイン
	case STAGING_FADEIN:
		// 色情報のセット
		m_uiInfo.color.w = m_fTime / m_fFadeSec;

		// 時間になったら演出終了
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;
	
	// フェードアウト
	case STAGING_FADEOUT:
		// 色情報のセット
		m_uiInfo.color.w = 1.0f - m_fTime / m_fFadeSec;

		// 時間になったら演出終了
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;

	case STAGING_FLASH:
		// 色情報のセット
		if (m_bFlashFadeIn)
			m_uiInfo.color.w = 1.0f - m_fTime / m_fFlashSpeed;
		else
			m_uiInfo.color.w = m_fTime / m_fFlashSpeed;

		// 時間になったら演出終了
		if (m_fTime > m_fFlashSpeed)
		{
			m_fTime = 0.0f;
			m_bFlashFadeIn = !m_bFlashFadeIn;
		}

		break;

	// ディゾルブ演出
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

// フェード処理
void CUI::Fade(STAGINGSTATE state, float fadesec)
{
	m_state = state;
	m_fTime = 0.0f;
	m_fFadeSec = fadesec;
}

// フラッシュ処理
void CUI::Flash(float flashSpeed)
{
	m_state = STAGING_FLASH;
	m_bFlashFadeIn = true;
	m_fFlashSpeed = flashSpeed;
}

// ディゾルブ処理
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
