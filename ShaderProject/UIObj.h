#pragma once
#include "Geometory.h"
#include "DirectX.h"
#include "UIList.h"
#include "Texture.h"

using namespace DirectX;


// UI描画に必要な情報
struct UIINFO
{
	XMFLOAT3 pos;
	XMFLOAT2 size;
	XMFLOAT4 color;
	Texture* pTex;
};

enum STAGINGSTATE
{
	STAGING_FADEIN = 0,		// フェードイン
	STAGING_FADEOUT,		// フェードアウト
	STAGING_FLASH,			// フラッシュ
	STAGING_DISSOLOVE,		// ディゾルブ演出
	STAGING_NONE,			// 演出無し
	
	STAGING_MAX
};

class CUI
{
public:
	CUI(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, Texture* pTex);
	~CUI();
	void Update();
	void Draw();
	void Fade(STAGINGSTATE state, float fadesec);
	void Flash(float flashSpeed);
	void Dissolve(float dissolvesec, Texture* pTex);
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 GetPos();
private:
	void Init();
private:
	UIINFO m_uiInfo;
	CGeometory* m_pGeometory;
	STAGINGSTATE m_state;		// フェードの状態
	float m_fFadeSec;			// 描画開始/終了時にフェードするかどうか
	float m_fFlashSpeed;		// 点滅スピード
	float m_fTime;
	float m_dissolveRate;
	float m_dissolveTime;
	bool m_bFlashFadeIn;	
	Texture* m_pTex;			// テクスチャ
	Texture* m_pDissolveTex;	// ディゾルブテクスチャ
};
