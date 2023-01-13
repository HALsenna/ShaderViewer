#include "GaussianBlur.h"
#include "TextureFactory.h"
#include <math.h>

const int NUM_WEIGHT = 8;	// ガウス関数の重みの数
const char* test = "Assets/Texture/TEX_Meat_02.png";

// 関数宣言
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

	//// レンダーターゲットの作成
	//// ブラー用のスプライトを初期化
	//InitSprite();

	// ガウスブラー用の重みテーブルの生成
	float weights[NUM_WEIGHT];
	CalcWeightsTable(weights, NUM_WEIGHT, 2.0f);

	// ブラー用のレンダリングターゲットを作成する
	InitRenderTargets();

	// スプライトの初期化
	InitSprites();
}

void GaussianBlur::Update()
{

}

void GaussianBlur::Draw()
{
	Texture* main;
	// テクスチャの設定
	//m_pxBlurSprite->SetTexture(メインレンダーターゲット->GetResource());
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

// レンダーターゲットの作成
void GaussianBlur::InitRenderTargets()
{
	m_xRenderTarget = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32G32B32_FLOAT, 1280.0f / 2.0f, 720.0f);
	m_yRenderTarget = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32G32B32_FLOAT, 1280.0f / 2.0f, 720.0f / 2.0f);
}

// スプライトの作成
void GaussianBlur::InitSprites()
{
	// ポリゴンの生成
	// 縦ブラー
	m_pxBlurSprite = new CGeometory();
	m_pxBlurSprite->InitGeometory();
	// 横ブラー
	m_pyBlurSprite = new CGeometory();
	m_pyBlurSprite->InitGeometory();
	// シェーダーの設定
	// サイズの設定
	// テクスチャの設定
	// 重みテーブルの設定
}

// 重みテーブルを計算する
void CalcWeightsTable(float* table, int sizeOfTable, float sigma)
{
	// 重みの合計を記録する変数
	float total = 0;

	// ガウス関数を用いて重みを計算
	for (int x = 0; x < sizeOfTable; ++x)
	{
		table[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * table[x];
	}

	// 重みの合計を除算し、重みの合計を1にする
	for (int i = 0; i < sizeOfTable; ++i)
	{
		table[i] /= total;
	}
}