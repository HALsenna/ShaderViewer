#pragma once

// ブレンドステート
enum EBlendState
{
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成

	MAX_BLENDSTATE
};
class CBlendState
{
public:
	static void Init();
	static void Uninit();
	static void SetBlendState(int nState);			// ブレンドステートの設定
	static void SetZBuffer(bool bEnable);
};
