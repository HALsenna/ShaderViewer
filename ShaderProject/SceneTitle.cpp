#include "SceneTitle.h"
#include "Model.h"
#include "TextureFactory.h"
#include "ShaderList.h"
#include "ConstantBuffer.h"
#include "CameraBase.h"
#include "CameraDCC.h"
#include "flower.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "Input.h"
#include "CBlendState.h"
#include "UIObj.h"
#include "Sound.h"
#include "Timer.h"
#include "SceneFade.h"
#include "SceneView.h"
#include "GaussianBlur.h"
#include "Light.h"
#include "SkyDome.h"

namespace
{
	const char* fileName = "Assets/Texture/TEX_Meat_02.png";
	const char* teapotFile = "Assets/Model/teapotSmooth.fbx";
	const char* houseFile = "Assets/Model/model_house_02.fbx";
	/*const char* skydomeFile_noon = "Assets/Model/SkyDome/NoonSky.fbx";
	const char* skydomeFile_evening = "Assets/Model/SkyDome/EveningSky.fbx";
	const char* skydomeFile_night = "Assets/Model/SkyDome/NightSky.fbx";*/
	const char* bridgeFile = "Assets/Model/model_bridge.fbx";
	const char* groundFile = "Assets/Model/model_moutain_02.fbx";
	const char* flowerpetalFile = "Assets/Model/flower/model_flower_petal_01.fbx";
	const char* flowerstemFile = "Assets/Model/flower/model_flower_stem_01.fbx";
}
const char* szUIFileName[TITLEUI_MAX]
{
	"Assets/Texture/Title/TEX_TitleLogoBG.png",	// タイトルロゴ(背景)
	"Assets/Texture/Title/TEX_Title.png",		// タイトルロゴ(名前)
	"Assets/Texture/Title/TEX_TitlePressKey.png",	// キー入力
	//"Assets/Texture/Title/TEX_Bane.png",	
};
const char* szDissolveTex = "Assets/Texture/dissolve.png";

const XMFLOAT3 TitleUIPos[TITLEUI_MAX]
{
	{0.0f, 50.0f, 0.0f},			// タイトルロゴ(背景)
	{0.0f, 50.0f, 0.0f},			// タイトルロゴ(名前)
	{0.0f, -100.0f, 0.0f},			// キー入力
	//{0.0f, -100.0f, 0.0f},			
};

const XMFLOAT2 TitleUISize[TITLEUI_MAX]
{
	{0.5f, 0.5f},	// タイトルロゴ
	{0.8f, 0.8f},	// タイトルロゴ
	{0.3f, 0.3f},	// キー入力
	//{0.5f, 0.5f},
};

const XMFLOAT4 UIColor[TITLEUI_MAX]
{
	{1.0f, 1.0f, 1.0f, 1.0f},		// タイトルロゴ(背景)
	{0.0f, 0.0f, 0.0f, 0.0f},		// タイトルロゴ(名前)
	{0.0f, 0.0f, 0.0f, 0.0f},		// キー入力
	//{1.0f, 1.0f, 1.0f, 1.0f},		// タイトルロゴ(背景)
};

// グローバル変数
//CPolygon* g_polygon;
float g_flowerColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float g_UIColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
CUI* g_pObjUI[TITLEUI_MAX];
Texture* g_pTex[TITLEUI_MAX];
CTimer* g_pTimer;
bool isStagingFin = false;
static SceneFade* g_pSceneFade;
static CLight* g_pLightManager;
static CSkyDome* g_pSkydome;
static float g_time;
//GaussianBlur* g_pBlur;

// プロトタイプ宣言
void SetVertexShader(ConstantBuffer* cb, CameraBase* camera, Model* model);
CUI** CreateUI();
void Bane();
//void StagingTitleLogo();
	                       
void SceneTitle::Init()
{
	m_pTitleScene = this;
	// カメラを固定する
	CameraDCC* pCameraDCC = GetObj<CameraDCC>("CameraDCC");

	// シーンフェードの設定
	g_pSceneFade = new SceneFade();
	g_pSceneFade->SetFade(SceneFade::FADE_IN, 1.0f);

	// ライトの設定
	g_pLightManager = new CLight();

	// オブジェクト情報の設定
	TitleObject obj;
	// タイマーの生成
	g_pTimer = new CTimer();
	g_time = 0.0f;
	// スカイドーム
	Model* pSkyModel[SKY_MAX] = {
		CreateObj<Model>("SkyDome_noon"),
		CreateObj<Model>("SkyDome_evening"),
		CreateObj<Model>("SkyDome_night"),
	};
	g_pSkydome = new CSkyDome(pSkyModel);

	// 背景モデル
	obj = { "MountainModel", groundFile, XMFLOAT3(0.0f, -2.0f, -5.0f),
		XMFLOAT3(0.0f, 90.0f, 0.0f), 0.005f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// 橋モデル
	obj = { "BridgeModel", bridgeFile, XMFLOAT3(4.0f, -2.0f, 7.0f),
		XMFLOAT3(0.0f, 90.0f, 0.0f), 0.3f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// 花モデル
	CFlower* flowerObj = new CFlower();
	obj = { "flowerpetalModel", flowerpetalFile, XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, VS_OBJECT, PS_FLOWER };
	SetObjInfo(obj);

	obj = { "flowerstemModel", flowerstemFile, XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// 花の色の定数バッファを設定する
	ConstantBuffer* pFlowerColorCB = CreateObj<ConstantBuffer>("CBFlowerColor");
	pFlowerColorCB->Create(sizeof(XMFLOAT4));

	// 環境光ライトの設定
	/*ConstantBuffer* pDirectionLightCB = CreateObj<ConstantBuffer>("CBDirectionalLight");
	pDirectionLightCB->Create(sizeof(XMFLOAT4) * 2);*/

	ConstantBuffer* pSkyDomeAlpha = CreateObj<ConstantBuffer>("CBSkyAlpha");
	pSkyDomeAlpha->Create(sizeof(XMFLOAT4));

	// モデルの生成
	//CreateModel();
	
	// テクスチャの生成
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pTex[i] = TextureFactory::CreateFromFile(szUIFileName[i]);
	}

	// --- UIの作成 ---
	CUI** cui = CreateUI();
	for(int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i] = cui[i];
	}
}
CUI** CreateUI()
{
	CUI* pUI[TITLEUI_MAX];
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		pUI[i] = new CUI(TitleUIPos[i], TitleUISize[i], UIColor[i], g_pTex[i]);
	}
	// タイマーセット
	g_pTimer->Set(2.0f, "TitleLogoTimer");		// 2秒後にタイトルロゴを出す
	g_pTimer->Set(7.0f, "KeyInputUITimer");		// 7秒後にキー入力UIを出す
	Texture* pTex = TextureFactory::CreateFromFile(szDissolveTex);
	pUI[TITLEUI_LOGO_BG]->Dissolve(5.0f, pTex);
	return pUI;
}

void SceneTitle::Uninit()
{
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i] = nullptr;
		g_pTex[i] = nullptr;
	}

	// シーンフェードの終了処理
	g_pSceneFade->Uninit();

	// ブラーの終了処理
	//g_pBlur->Uninit();

	// BGMの停止
	CSound::Stop(BGM_TITLE);
}
void SceneTitle::Update()
{
	//　ライトの更新
	XMFLOAT3 color;
	color = g_pLightManager->GetSkyColor();
	g_pLightManager->Update();

	// 空の更新
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	g_pSkydome->SetCamera(pCamera, pWVP);
	g_pSkydome->Update();

	SKYKIND kind = g_pSkydome->GetSkyKind();
	XMFLOAT3 _color = g_pLightManager->GetSkyColor();
	XMFLOAT3 _dir = g_pLightManager->GetSkyLightDir();
	float colorChange = 0.5f * (1.0f / 60.0f) / TIME_SKYCHANGE;
	float dirChange = 1.0f * (1.0f / 60.0f) / TIME_SKYCHANGE;

	g_time += 1.0f / 60.0f;
	if (g_time > TIME_SKYCHANGE)
	{
		g_time = 0.0f;
	}

	switch (kind)
	{
	case SKY_NOON:
		_color.x += colorChange;
		_dir.y += dirChange;
		break;
	case SKY_EVENING:
		_color.x = 0.0f;
		_color.y += colorChange;
		_dir.y += dirChange;
		break;
	case SKY_NIGHT:
		_color.y = 0.0f;
		_dir.y -= dirChange * 2;
		break;
	}

	g_pLightManager->SetSkyColor(_color);
	g_pLightManager->SetSkyLightDir(_dir);

	// 花の色を変更する
	ImGui::ColorEdit4(u8"MudSphereColor", g_flowerColor);

	if (IsKeyPress(VK_LEFT))
		pos.y -= 0.1f;
	if (IsKeyPress(VK_RIGHT))
		pos.y += 0.1f;

	// Enterキー→ゲームシーンに遷移する
	if (IsKeyPress(VK_RETURN))
		g_pSceneFade->SetFade(SceneFade::FADE_OUT, 1.0f);

	// タイマーの更新
	g_pTimer->Update();

	// タイトルロゴの演出の更新
	if (g_pTimer->isTimerFin("TitleLogoTimer"))
	{
		g_pObjUI[TITLEUI_LOGO]->Fade(STAGING_FADEIN, 5.0f);
		CSound::Play(BGM_TITLE);		
		g_pTimer->Reset("TitleLogoTimer");
	}

	if (g_pTimer->isTimerFin("KeyInputUITimer"))
	{
		g_pObjUI[TITLEUI_KEYINPUT]->Flash(2.0f);
		g_pTimer->Reset("KeyInputUITimer");
	}

	//Bane();

	// UIの更新
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i]->Update();
	}

	// シーンフェード更新
	g_pSceneFade->Update();

	// ブラー更新
	//g_pBlur->Update();
}
#pragma region draw
void SceneTitle::Draw()
{
	// 定数バッファの取得
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pFlowerColorCB = GetObj<ConstantBuffer>("CBFlowerColor");
	ConstantBuffer* pDirectionalLightCB = GetObj<ConstantBuffer>("CBDirectionalLight");
	
	// 環境光の設定
	//XMFLOAT4 lightDir(-1.0f, 1.0f, 1.0f, 0.0f);
	//pLight->Write(&lightDir);
	//pLight->BindPS(0);
	XMFLOAT3 color = g_pLightManager->GetSkyColor();
	XMFLOAT3 dir = g_pLightManager->GetSkyLightDir();
	XMFLOAT4 light[] = {
		XMFLOAT4{dir.x, dir.y, dir.z, 0.0f},
		XMFLOAT4{color.x, color.y, color.z, 0.0f},
	};
	pDirectionalLightCB->Write(&light);
	pDirectionalLightCB->BindPS(0);
	
	for (int i = 0; i < m_sceneObj.size(); ++i)
	{
		// 頂点シェーダーの設定
		SetVertexShader(pWVP, pCamera, m_sceneObj[i].pModel);

		// 花びらの色を変える
		if (m_sceneObj[i].name == "flowerpetalModel")
		{
			pFlowerColorCB->Write(g_flowerColor);
			pFlowerColorCB->BindPS(0);
		}
		// オブジェクトの描画
		GetObj<Model>(m_sceneObj[i].name)->Draw();
	}

	// 空の描画
	g_pSkydome->Draw();

	// アルファブレンド設定
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);

	// UI描画
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i]->Draw();
	}

	// シーンフェード描画
	g_pSceneFade->Draw();
	CBlendState::SetZBuffer(true);
}
#pragma endregion 

// オブジェクト情報の設定
#pragma region set obj info
void SceneTitle::SetObjInfo(TitleObject obj)
{
	Model* pModel = CreateObj<Model>(obj.name);
	pModel->Load(obj.fileName, obj.size);
	pModel->SetVertexShader(GetVS(obj.vs));
	pModel->SetPixelShader(GetPS(obj.ps));
	pModel->m_vertex.pos = obj.pos;
	pModel->m_vertex.rot = obj.rot;
	obj.pModel = pModel;
	m_sceneObj.push_back(obj);
}
#pragma endregion 

// 頂点シェーダーの設定
void SetVertexShader(ConstantBuffer* cb, CameraBase* camera, Model* model)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	DirectX::XMFLOAT4X4 mat[3];

	// --- ワールド座標情報の格納 ---
	// 回転
	worldMatrix = XMMatrixRotationX(XMConvertToRadians(model->m_vertex.rot.x));
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(model->m_vertex.rot.y));
	worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(model->m_vertex.rot.z));

	// 座標変換
	worldMatrix *= XMMatrixTranslation(model->m_vertex.pos.x, model->m_vertex.pos.y, model->m_vertex.pos.z);
	//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));
	// --------------------------
	mat[1] = camera->GetView();
	mat[2] = camera->GetProj();
	cb->Write(mat);
	cb->BindVS(0);
}

void Bane()
{
	//zg_pObjUI[TITLEUI_BANE]->SetPos(XMFLOAT3(0.0f, 100.0f, 0.0f));
}
