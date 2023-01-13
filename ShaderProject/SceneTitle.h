#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneBase.hpp"
#include "ShaderList.h"
#include "Model.h"
#include <vector>
#include <DirectXMath.h>
#include "UIObj.h"

using namespace DirectX;
// オブジェクト情報
struct TitleObject
{
	const char* name;		// 名前
	const char* fileName;	// 読込ファイルの名前
	XMFLOAT3 pos;			// 生成時の座標
	XMFLOAT3 rot;			// 生成時の回転
	float size;				// 生成時の大きさ
	VertexShaderKind vs;	// 頂点シェーダー
	PixelShaderKind ps;		// ピクセルシェーダー
	Model* pModel;			// モデル情報
};

class SceneTitle : public SceneBase
{
// メンバ関数
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetObjInfo(TitleObject obj);

private:
	//void CreateModel();

// メンバ変数
private:
	std::vector<TitleObject> m_sceneObj;		// シーンオブジェクト
public:
	SceneTitle* m_pTitleScene;
};

#endif // __SCENE_SHADING_H__