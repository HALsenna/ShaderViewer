#pragma once
#include <DirectXMath.h>
#include "ShaderList.h"
#include "Model.h"

struct SCENEOBJECTINFO
{
	const char* name;			// 名前
	const char* fileName;		// 読込ファイルの名前
	DirectX::XMFLOAT3 pos;		// 座標
	DirectX::XMFLOAT3 rot;		// 回転
	XMFLOAT3 size;					// サイズ
	VertexShaderKind vs;	// 頂点シェーダー
	PixelShaderKind ps;	// ピクセルシェーダー
	Model* pModel;				// モデルデータ
};
