#include "SceneSelect.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "CBlendState.h"
#include "CameraBase.h"
#include "Input.h"

// 定数定義
namespace
{
	const float BUTTON_SIZE = 0.4f;
	const float KEYUI_SIZE = 0.3f;
	const float KEYUI_POSY = -300.0f;
	const float KEYUI_CENTER_POSX = -200.0f;
	const float KEYUI_INTERVAL = 50.0f;
}

// ---- UIの情報 ----
#pragma region UIInitInfo
// ファイル名
static const char* szUIFileName[SELECT_MAX] = {
	"Assets/Texture/MenuUI/TEX_BG_Plane.png",		// 背景
	"Assets/Texture/MenuUI/TEX_Menu_Meat.png",		// 選択項目(お肉)
	"Assets/Texture/MenuUI/TEX_Menu_UI.png",		// 選択項目(UI)
	"Assets/Texture/MenuUI/TEX_Setting_BG.png",		// 設定項目(背景)
	"Assets/Texture/MenuUI/TEX_Button_Decide.png",	// 決定ボタン
	"Assets/Texture/MenuUI/TEX_Key_1.png",			// キーNum1
	"Assets/Texture/MenuUI/TEX_Key_2.png",			// キーNum2
	"Assets/Texture/MenuUI/TEX_Key_3.png",			// キーNum3
	"Assets/Texture/MenuUI/TEX_ChangeLight.png",	// ライト切替UI
};

// UI情報
const UIINFO g_UIInfo[SELECT_MAX] = {
	{{0.0f, 0.0f, 0.0f},{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// 背景
	{{-500.0f, 250.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// 選択項目(お肉)
	{{-500.0f, 180.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// 選択項目(UI)
	{{400.0f, 50.0f, 0.0f},{0.4f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},	// 選択項目(背景)
	{{400.0f, -100.0f, 0.0f},{0.4f, 0.4f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},	// 決定ボタン
	{{KEYUI_CENTER_POSX - KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// キーNum1
	{{KEYUI_CENTER_POSX, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},						// キーNum2
	{{KEYUI_CENTER_POSX + KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// キーNum3
	{{50.0f, KEYUI_POSY, 0.0f},{0.7f, 0.7f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// ライト切替UI
};
#pragma endregion
// ------------------

// ---- モデルの初期情報 ----
#pragma region ModelInitInfo
// オブジェクト名
const char* g_szObjectName[SELECTOBJ_MAX] = {
	"Meat",		// お肉
};

// 座標
const XMFLOAT3 g_ObjectPos[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// お肉
};

// 回転
const XMFLOAT3 g_ObjectRot[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// お肉
};

// サイズ
const XMFLOAT3 g_ObjectSize[SELECTOBJ_MAX] = {
	{1.0f, 1.0f, 1.0f},					// お肉
};

// 頂点シェーダー
const VertexShaderKind g_vShader[SELECTOBJ_MAX] = {
	VS_OBJECT,				// お肉
};

// ピクセルシェーダー
const PixelShaderKind g_pShader[SELECTOBJ_MAX] = {
	PS_OBJECT,				// お肉
};
#pragma endregion 
// -----------------------

// ---- 入力キー ----
#pragma region inputkey
namespace
{
	const BYTE KEY_CHANGELIGHT_NORMAL = '1';	// 通常状態
	const BYTE KEY_CHANGELIGHT_INSIDE =	'2';	// 建物内
	const BYTE KEY_CHANGELIGHT_NIGHT = '3';		// 夜	
}
#pragma endregion 
// ----------------

// グローバル変数
static CMeat_Setting* g_pMeatObj;		// お肉オブジェクト

// プロトタイプ宣言
UIINFO SetUpUI(UIINFO);
CUI* CreateUI(UIINFO);

void SceneSelect::Init()
{
	// UIの初期化
	InitUI();
	// 3Dの初期化
	Init3D();
	// 環境光の初期化
	InitLight();
	
}
void SceneSelect::Uninit()
{
	m_pObjData.clear();
	m_pObjData.clear();
}
void SceneSelect::Update()
{
	// UIの更新
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		m_pUIObj[i]->Update();
	}

	// キー入力受付更新
	UpdateKeyInput();
	// お肉更新
	g_pMeatObj->Update();
}
void SceneSelect::Draw()
{
	// アルファブレンド設定
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);
	
	// 背景描画
	m_pUIObj[SELECT_BG_PLANE]->Draw();
	
	// アルファブレンド設定を元に戻す
	CBlendState::SetZBuffer(true);
	
	// 3Dオブジェクトの描画
	for (int i = 0; i < m_pObjData.size(); ++i)
	{
		SetShader((SELECTSCENE_OBJECT) i);			// シェーダーの設定
		m_pObjData[i]->Draw();
	}

	// アルファブレンド設定
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);

	// UIの描画
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// 背景以外を描画する
		if (i != (int)SELECT_BG_PLANE)
			m_pUIObj[i]->Draw();
	}
	
	// アルファブレンド設定を元に戻す
	CBlendState::SetZBuffer(true);
}

/*
	@brief UIの初期化
	@details UIの座標、色、テクスチャを設定し、オブジェクトを生成する
*/
void SceneSelect::InitUI()
{
	Texture* pTex;
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// UIの初期設定
		m_aUIInitData[i] = SetUpUI(g_UIInfo[i]);
		pTex = TextureFactory::CreateFromFile(szUIFileName[i]);
		m_aUIInitData[i].pTex = pTex;
		// UIの生成
		m_pUIObj[i] = CreateUI(m_aUIInitData[i]);
	}
}

/*
	@brief モデルの初期化
	@details モデルの座標を生成する
*/
void SceneSelect::InitModel()
{
	SCENEOBJECTINFO obj;		// 情報格納用
	
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		obj.name = g_szObjectName[i];	// オブジェクト名前
		obj.pos = g_ObjectPos[i];		// 座標
		obj.rot = g_ObjectRot[i];		// 回転
		obj.size = g_ObjectSize[i];		// サイズ
		obj.vs = g_vShader[i];			// 頂点シェーダー
		obj.ps = g_pShader[i];			// ピクセルシェーダー
		
		// 情報のセット
		m_ObjInitData.push_back(obj);
	}
}

// モデルの生成
void SceneSelect::CreateModel()
{
	Model* pModel;		// 情報格納用
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		pModel = CreateObj<Model>(g_szObjectName[i]);
		m_ObjInitData[i].pModel = pModel;
		m_pObjData.push_back(pModel);
	}
}

// 環境光の初期化
void SceneSelect::InitLight()
{
	m_pSceneLight = new CSelectSceneLight();
}

// UIの初期設定
UIINFO SetUpUI(UIINFO info)
{
	UIINFO pData;
	pData.pos = info.pos;			// 座標
	pData.size = info.size;			// サイズ
	pData.color = info.color;		// 色
	//pData.pTex = info.pTex;			// テクスチャ
	return pData;
}

// UIの生成
CUI* CreateUI(UIINFO data)
{
	CUI* pUI;
	pUI = new CUI(data.pos, data.size, data.color, data.pTex);
	return pUI;
}

void SceneSelect::UpdateKeyInput()
{
	// ライト変更(通常状態)
	if (IsKeyPress(KEY_CHANGELIGHT_NORMAL))
	{
		m_pSceneLight->ChangeLight(LIGHT_NORMAL);
	}
	// ライト変更(建物内)
	else if (IsKeyPress(KEY_CHANGELIGHT_INSIDE))
	{
		m_pSceneLight->ChangeLight(LIGHT_INSIDE);
	}
	// ライトの変更(夜)
	else if (IsKeyPress(KEY_CHANGELIGHT_NIGHT))
	{
		m_pSceneLight->ChangeLight(LIGHT_NIGHT);
	}
}

// 3Dの初期化
void SceneSelect::Init3D()
{
	// モデルの初期化
	InitModel();
	// モデルの生成
	CreateModel();
	// オブジェクトの生成
	CreateObject();
}

// オブジェクトの生成
void SceneSelect::CreateObject()
{
	// お肉オブジェクト
	g_pMeatObj = new CMeat_Setting(m_ObjInitData[SELECTOBJ_MEAT]);
}

// シェーダーの設定
void SceneSelect::SetShader(SELECTSCENE_OBJECT kind)
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pDirectionalLightCB = GetObj<ConstantBuffer>("CBDirectionalLight");

	// ---- 頂点シェーダーの設定 ----
	XMMATRIX mWorld;		// ワールド座標
	XMFLOAT4X4 mat[3];

	// ワールド変換
	// 回転
	mWorld = XMMatrixRotationX(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.x));
	mWorld *= XMMatrixRotationY(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.y));
	mWorld *= XMMatrixRotationZ(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.z));

	// 座標変換
	mWorld *= XMMatrixTranslation(m_pObjData[kind]->m_vertex.pos.x, m_pObjData[kind]->m_vertex.pos.y, m_pObjData[kind]->m_vertex.pos.z);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(mWorld));

	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);
	// --------------------------

	// ---- ピクセルシェーダーの設定 ----
	XMFLOAT3 dir = m_pSceneLight->GetDir();
	XMFLOAT4 color = m_pSceneLight->GetColor();
	XMFLOAT4 light[] = {
		{dir.x, dir.y, dir.z, 0.0f},
		color
	};
	pDirectionalLightCB->Write(&light);
	pDirectionalLightCB->BindPS(0);
	// --------------------------
}
