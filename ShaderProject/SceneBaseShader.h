#ifndef __SCENE_BASESHADER_H__
#define __SCENE_BASESHADER_H__

#include "SceneBase.hpp"
#include "ConstantBuffer.h"
#include "Model.h"
#include "CameraBase.h"
#include <vector>
#include "ObjectsList.h"
#include "CameraDepth.h"
#include "Food.h"

using namespace DirectX;
using namespace std;

class SceneBaseShader : public SceneBase
{
public:
	enum KINDSHADER
	{
		SHADER_OBJECT = 0,		// オブジェクトを表示するだけ
		SHADER_DIRECTIONLIGHT,	// ディレクションライト
		SHADER_MAX,
	};

	enum IMGUIFLG
	{
		CB_DIRTIONLIGHT = 0,	// ディレクションライト(チェックボックス)
		IMGUI_MAX,
	};

private:
	struct DirectionLight
	{
		XMFLOAT4 lightDir;		// ライトの方向
		XMFLOAT4 lightColor;	// ライトのカラー
	};

	enum BUMPMODEL
	{
		BUMPMODEL_ROCK,
		BUMPMODEL_TENT,
		BUMPMODEL_MAX
	};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void InitLight();
	void InitModel();
	void SetDirectionLight();	// 拡散光の設定
	void SetSpecular();			// 鏡面反射光の設定
	void SetCamera(CameraBase* _pCamera, ConstantBuffer* _pCameraBuf);			// カメラ
	void DrawDirectionLight(ConstantBuffer* _pLight);	// 拡散光の描画
	void DrawSpecular(ConstantBuffer* _pLight);	// 拡散光の描画
	void DrawField(XMMATRIX worldMatrix, int index, XMFLOAT4X4 mat, ConstantBuffer* _pWVP, Texture* _pTex,
		ConstantBuffer* _pLight, CameraBase* _pCamera, ConstantBuffer* _pCamBuf);

private:
	float m_rad;
	float m_seaLightRad;
	float m_MudColor[4] = { 0.45f, 0.3f, 0.18f, 1.0f };
	float m_SeaColor[4] = { 0.37f, 0.5f, 0.87f, 1.0f };
	float m_rate;
	XMVECTOR vecCamera;

	bool m_isView[SHADER_MAX];		// シェーダー表示中か
	bool m_ImGUIFlg[IMGUI_MAX];		// ImGUI用フラグ

	DirectionLight m_directionLight;	// ディレクションライト
	PointLight* m_PointLight;			// ポイントライト
	float m_time;

	//OBJECTLIST m_sceneObj;
	Model* m_pSceneModel[OBJ_MAX];			// モデル情報
	MeshBuffer** m_pBumpModel[BUMPMODEL_MAX];			// モデル情報
	CameraDepth* m_cameraDepth;

	CFood* pFoodManager;
};

#endif // __SCENE_BASESHADER_H__
