#include "CameraDepth.h"
#include "SceneBase.hpp"


CameraDepth::CameraDepth(Model* sprite)
{
	// ガウシアンブラーオブジェクトを作成
	m_gaussian = new GaussianBlur();
	m_pSprite = sprite;
}

CameraDepth::~CameraDepth()
{
}

void CameraDepth::Init(RenderTarget* defRTV)
{
	m_gaussian->Init(defRTV);

	// ボケ画像合成用スプライトの初期化
	//m_pSprite


}

void CameraDepth::Update()
{
}

void CameraDepth::Draw()
{
}
