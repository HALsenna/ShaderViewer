#include "GaussianBlur.h"
#include "TextureFactory.h"
#include <math.h>

const int NUM_WEIGHT = 8;	// �K�E�X�֐��̏d�݂̐�
const char* test = "Assets/Texture/TEX_Meat_02.png";

// �֐��錾
void CalcWeightsTable(float* table, int sizeOfTable, float sigma);

GaussianBlur::GaussianBlur()
{
}

GaussianBlur::~GaussianBlur()
{
}

void GaussianBlur::Init(Texture* tex)
{
	m_texture = tex;

	//// �����_�[�^�[�Q�b�g�̍쐬
	//// �u���[�p�̃X�v���C�g��������
	//InitSprite();

	// �K�E�X�u���[�p�̏d�݃e�[�u���̐���
	float weights[NUM_WEIGHT];
	CalcWeightsTable(weights, NUM_WEIGHT, 2.0f);

	// �u���[�p�̃����_�����O�^�[�Q�b�g���쐬����
	InitRenderTargets();

	// �X�v���C�g�̏�����
	InitSprites();
}

void GaussianBlur::Update()
{

}

void GaussianBlur::Draw()
{
	Texture* main;
	// �e�N�X�`���̐ݒ�
	//m_pxBlurSprite->SetTexture(���C�������_�[�^�[�Q�b�g->GetResource());
	m_pxBlurSprite->SetTexture(m_xRenderTarget->GetResource());
	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT2 size = { 1280.0f, 720.0f };
	XMFLOAT3 rot = { 0.0f, 0.0f, 0.0f };
	m_pxBlurSprite->DrawPolygon(pos, size, rot);
	m_pyBlurSprite->DrawPolygon(pos, size, rot);
}

void GaussianBlur::Uninit()
{
	m_pxBlurSprite->UninitGeometory();
	m_pyBlurSprite->UninitGeometory();
}

// �����_�[�^�[�Q�b�g�̍쐬
void GaussianBlur::InitRenderTargets()
{
	m_xRenderTarget = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32G32B32_FLOAT, 1280.0f / 2.0f, 720.0f);
	m_yRenderTarget = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32G32B32_FLOAT, 1280.0f / 2.0f, 720.0f / 2.0f);
}

// �X�v���C�g�̍쐬
void GaussianBlur::InitSprites()
{
	// �|���S���̐���
	// �c�u���[
	m_pxBlurSprite = new CGeometory();
	m_pxBlurSprite->InitGeometory();
	// ���u���[
	m_pyBlurSprite = new CGeometory();
	m_pyBlurSprite->InitGeometory();
	// �V�F�[�_�[�̐ݒ�
	// �T�C�Y�̐ݒ�
	// �e�N�X�`���̐ݒ�
	// �d�݃e�[�u���̐ݒ�
}

// �d�݃e�[�u�����v�Z����
void CalcWeightsTable(float* table, int sizeOfTable, float sigma)
{
	// �d�݂̍��v���L�^����ϐ�
	float total = 0;

	// �K�E�X�֐���p���ďd�݂��v�Z
	for (int x = 0; x < sizeOfTable; ++x)
	{
		table[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * table[x];
	}

	// �d�݂̍��v�����Z���A�d�݂̍��v��1�ɂ���
	for (int i = 0; i < sizeOfTable; ++i)
	{
		table[i] /= total;
	}
}