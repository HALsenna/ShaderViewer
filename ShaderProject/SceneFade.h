#pragma once
#include "Geometory.h"
#include "Texture.h"

class SceneFade
{
public:
	enum FADESTATE
	{
		FADE_IN = 0,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_NONE,			// 何もしていない状態
		FADE_FIN,		// フェード終了		
		FADE_MAX,
	};
public:
	SceneFade();
	~SceneFade();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetFade(FADESTATE state, float seconds );
	void Reset();
	bool IsFadeFin();
private:
	CGeometory* pGeometory;
	Texture* m_pFadeTex;		// フェード用のテクスチャ
	FADESTATE m_state;
	float m_fCurrentTime;		// 現在のカウント
	float m_fTexAlpha;			// テクスチャ描画アルファ値
	float m_fFadeSec;			// フェード時間
};
