#pragma once

enum TITLEUIKIND
{
	TITLEUI_LOGO_BG = 0,	// タイトルロゴ(背景)
	TITLEUI_LOGO,			// タイトルロゴ(名前)
	TITLEUI_KEYINPUT,		// キー入力
	//TITLEUI_BANE,
	TITLEUI_MAX
};

enum SELECTUIKIND
{
	SELECT_BG_PLANE = 0,		// 背景
	SELECT_MEAT,		// 選択項目(肉)
	SELECT_UI,			// 選択項目(UI)
	SELECT_SETTING_BG,	// 設定項目(背景)
	SELECT_DECIDE,		// 選択項目(決定ボタン)
	SELECT_KEY_1,		// Num1キー
	SELECT_KEY_2,		// Num2キー
	SELECT_KEY_3,		// Num3キー
	SELECT_CHANGE_LIGHT,	// ライト変更文字
	SELECT_MAX,
};
