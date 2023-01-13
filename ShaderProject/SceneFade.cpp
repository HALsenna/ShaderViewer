#include "SceneFade.h"
#include "TextureFactory.h"

const char* szFadeTextureFile = "Assets/Texture/Fade/TEX_Fade.png";

SceneFade::SceneFade()
{
	pGeometory = new CGeometory();
	Init();
	pGeometory->InitGeometory();
}

SceneFade::~SceneFade()
{
	delete pGeometory;
}

void SceneFade::Init()
{
	m_pFadeTex = TextureFactory::CreateFromFile(szFadeTextureFile);
	m_fTexAlpha = 0.0f;
	m_fFadeSec = 0.0f;
	m_fCurrentTime = 0.0f;
	m_state = FADE_NONE;
}

void SceneFade::Uninit()
{

}

void SceneFade::Update()
{
	m_fCurrentTime += 1.0f / 60.0f;
	switch (m_state)
	{
		// フェードイン
		case FADE_IN:
			// α値の更新
			m_fTexAlpha = 1.0f - m_fCurrentTime / m_fFadeSec;

			// 時間になったら演出終了
			if (m_fCurrentTime > m_fFadeSec)
			{
				m_state = FADE_FIN;
				Reset();
			}
			break;
		case FADE_OUT:
			// α値の更新
			m_fTexAlpha = m_fCurrentTime / m_fFadeSec;

			// 時間になったら演出終了
			if (m_fCurrentTime > m_fFadeSec)
			{
				m_state = FADE_FIN;
				Reset();
			}
			break;
		case FADE_FIN:
			break;
		default:
			break;
	}

	
}

void SceneFade::Draw()
{
	pGeometory->SetTexture(m_pFadeTex->GetResource());
	pGeometory->SetAlpha(m_fTexAlpha);
	pGeometory->DrawPolygon(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1285, 725), XMFLOAT3(0.0f, 0.0f, 0.0f));
}

void SceneFade::SetFade(FADESTATE state, float seconds)
{
	m_state = state;
	m_fFadeSec = seconds;

	if (state == FADE_IN)
		m_fTexAlpha = 1.0f;
	else if (state == FADE_OUT)
		m_fTexAlpha = 0.0f;
}

void SceneFade::Reset()
{
	m_fCurrentTime = 0.0f;
	m_fFadeSec = 0.0f;
}

// フェード終了通知
bool SceneFade::IsFadeFin()
{
	if (m_state == FADE_FIN)
		return true;
	else
		return false;
}

