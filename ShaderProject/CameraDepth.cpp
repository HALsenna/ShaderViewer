#include "CameraDepth.h"
#include "SceneBase.hpp"


CameraDepth::CameraDepth(Model* sprite)
{
	// �K�E�V�A���u���[�I�u�W�F�N�g���쐬
	m_gaussian = new GaussianBlur();
	m_pSprite = sprite;
}

CameraDepth::~CameraDepth()
{
}

void CameraDepth::Init(RenderTarget* defRTV)
{
	m_gaussian->Init(defRTV);

	// �{�P�摜�����p�X�v���C�g�̏�����
	//m_pSprite


}

void CameraDepth::Update()
{
}

void CameraDepth::Draw()
{
}
