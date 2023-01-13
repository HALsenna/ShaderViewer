#pragma once
#include <DirectXMath.h>

// 環境光の種類
enum KIND_LIGHT_SELECTSCENE
{
	LIGHT_NORMAL = 0,		//　通常状態
	LIGHT_INSIDE,			// 建物内
	LIGHT_NIGHT,			// 夜
	
	LIGHT_MAX
};

// セレクトシーンの環境光
class CSelectSceneLight
{
// メンバー関数
public:
	CSelectSceneLight();
	~CSelectSceneLight();
	void Init();
	void Update();
	DirectX::XMFLOAT3 GetDir();
	DirectX::XMFLOAT4 GetColor();
	void ChangeLight(KIND_LIGHT_SELECTSCENE kind);		// ライトを変更する

// メンバー変数
private:
	KIND_LIGHT_SELECTSCENE m_currentKind;		// 現在表示しているライトの種類
	DirectX::XMFLOAT3 m_dir;		// ライトの方向
	DirectX::XMFLOAT4 m_color;		// ライトの色

};
