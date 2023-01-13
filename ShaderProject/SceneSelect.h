#ifndef __SCENE_SELECT_H__
#define __SCENE_SELECT_H__

#include "SceneBase.hpp"
#include "UIObj.h"			// UI描画
#include "UIList.h"			// UIの種類
#include "SettingObjectsList.h"		// オブジェクトの種類
#include "ShaderList.h"
#include <vector>
#include "Model.h"
#include "SceneObject.h"
#include "SettingSceneLight.h"		// 環境光

class SceneSelect : public SceneBase
{
	// メンバ関数
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void Init3D();		// 3Dの初期化
	void InitUI();		// UIの初期化
	void InitModel();	// モデルの初期化
	void CreateModel();	// モデルの生成
	void InitLight();	// 環境光の初期化
	void CreateObject();	// オブジェクトの生成
	void SetShader(SELECTSCENE_OBJECT kind);	// シェーダー情報のセット
	void UpdateKeyInput();

	// メンバ変数
private:
	CUI* m_pUIObj[SELECT_MAX];
	UIINFO m_aUIInitData[SELECT_MAX];			// UIの初期データ
	std::vector<SCENEOBJECTINFO> m_ObjInitData;	// シーンオブジェクトの初期データ
	std::vector<Model*> m_pObjData;				// シーンオブジェクトのデータ
	CSelectSceneLight* m_pSceneLight;					// シーンの環境光
	//UIINFO  m_aUIInitData[SELECT_MAX];		// UIの初期データ
};

#endif // __SCENE_SHADING_H__