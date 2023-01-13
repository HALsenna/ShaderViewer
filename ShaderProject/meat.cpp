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
	XMFLOAT3 lightColor;		// ライトの色
	XMFLOAT3 lightDir;			// ライトの方向
};

// モデルのファイル名
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
	// 演出BGMを格納する
	m_stagingBGM[0] = BGM_PERFECT;
}

void Cmeat::Update()
{
	if (IsKeyTrigger('M'))
	{
		switch (m_stagingState)
		{
			// 待機状態の場合
			case STATE_IDLE:
				// 焼き状態にする
				ChangeState(STATE_GRILLD);
				break;

			// 焼き状態の場合
			case STATE_GRILLD:
				// 焼き具合評価に移る
				ChangeState(STATE_STAGING);
				break;
			// 演出状態の場合
			case STATE_STAGING:
				// 特に何もしない
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
	// 待機状態
	case Cmeat::STATE_IDLE:
		break;
	// 焼く
	case Cmeat::STATE_GRILLD:
		CSound::Play(BGM_COOKING);			// 調理BGMの再生
		CSound::SetVolume(BGM_COOKING, 3.0f);
		CSound::Play(BGM_GRILL);			// 調理BGMの再生
		CSound::SetVolume(BGM_GRILL, 0.5f);
		break;
	case Cmeat::STATE_STAGING:
		CSound::Stop(BGM_COOKING);			// 調理BGMの停止
		CSound::Stop(BGM_GRILL);			// 調理BGMの再生

		// 結果に応じたBGMを再生する
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
		// 待機状態
		case Cmeat::STATE_IDLE:
			// 特に何もしない
			XMFLOAT4 cb = { specular, 0.0f, 0.0f, 0.0f };
			m_specularCB->Write(&cb);
			m_specularCB->BindPS(3);
		break;
		// 焼く
		case Cmeat::STATE_GRILLD:
			m_condition += 0.1f;
			ImGui::Text("cooking:%.2f", m_condition);
			m_pModel->m_vertex.rot.x += 0.5f;
			m_pBoneModel->m_vertex.rot.x += 0.5f;
			
			// ピクセルシェーダーの設定
			cb = { m_condition, m_maxCookTime, 0.0f, 0.0f };
			m_cb->Write(&cb);
			m_cb->BindPS(2);
			cb = { specular, 0.0f, 0.0f, 0.0f };
			m_specularCB->Write(&cb);
			m_specularCB->BindPS(3);

			if (m_condition > m_maxCookTime + 30.0f)
				ChangeState(STATE_STAGING);
		break;

		// 結果演出中
		case Cmeat::STATE_STAGING:
			if (!CSound::IsPlaying(BGM_PERFECT))
			{
				CSound::Stop(BGM_PERFECT);
				// 焼け具合をリセットする
				m_condition = 0.0f;
				// 角度をリセットする
				m_vertex.rot = { 0.0f, 0.0f, 0.0f };
				// 肉の色を設定する
				cb = { m_condition, m_maxCookTime, 0.0f, 0.0f };
				m_cb->Write(&cb);
				m_cb->BindPS(2);
				cb = { specular, 0.0f, 0.0f, 0.0f };
				m_specularCB->Write(&cb);
				m_specularCB->BindPS(3);
				// 待機状態に戻す
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

	// パーフェクト状態
	if (m_condition >= m_maxCookTime && m_condition < m_maxCookTime + 5.0f)
	{
		ev = EVA_GRILLD;
	}
	// 生焼け状態
	else if (m_condition < m_maxCookTime)
	{
		ev = EVA_RARE;
	}
	// 焦げ状態
	else if (m_condition >= m_maxCookTime + 5.0f)
	{
		ev = EVA_BURNT;
	}

	return ev;
}
