#include "SceneBaseShader.h"
#include "TextureFactory.h"
#include "ShaderList.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "ObjectsList.h"
#include "SceneView.h"
#include "DepthStencil.h"
#include "Sprite.h"
#include "Sound.h"
#include "CameraDCC.h"
#include "Shop.h"
#include "Geometory.h"

using namespace DirectX;

Cmeat* meatObj;
CPlayer* playerObj;
CTree* treeObj;
CChair* chairObj;
Ctent* tentObj;
Cfire* fireObj;
CBonfire* bonfireObj;
CMountain* mountainObj;
CRock* rockObj;
CSkyDome* skyObj;
PointLight* ptLightObj;

struct BumpVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 tangent;		// 接ベクトル(タンジェントベクトル)
	// ワールド空間上でテクスチャを貼り付ける横の方向
};

// グローバル変数
static ConstantBuffer* pDefLight;
static bool bOldImgFlg = false;
float fRot = 0.0f;
bool isCooking = false;
static CGeometory* g_pGeometory;

bool isDebug = true;
// プロトタイプ宣言
MeshBuffer** Create_BumpModel(Model* _pModel);

#pragma region init
void SceneBaseShader::Init()
{
	// タイマーの初期化
	m_time = 0.0f;

	// シェーダー表示フラグの初期化
	#pragma region init flag array
	for (int i = 0; i < SHADER_MAX; ++i)
	{
		m_isView[i] = false;
	}
	m_isView[SHADER_DIRECTIONLIGHT] = true;
	for (int i = 0; i < IMGUI_MAX; ++i)
	{
		m_ImGUIFlg[i] = false;
	}
	#pragma endregion 

	// BGM再生
	CSound::Play(BGM_ENVIRONMENT);

	// モデルの設定
	#pragma region set model
	InitModel();
	#pragma endregion 

	// 定数バッファの設定
	#pragma region set constant buffer
	// カメラの生成
	#pragma region set camera
	// カメラ
	ConstantBuffer* pCamBuf = CreateObj<ConstantBuffer>("CameraCB");
	pCamBuf->Create(sizeof(DirectX::XMFLOAT4) * 2);
	// ライトの色
	ConstantBuffer* pLight = CreateObj<ConstantBuffer>("LightCB");
	pLight->Create(sizeof(XMFLOAT4) * 4);

	// お肉の色
	ConstantBuffer* pMeatCondition = CreateObj<ConstantBuffer>("MeatCB");
	pMeatCondition->Create(sizeof(XMFLOAT4));
	meatObj->m_cb = pMeatCondition;
	// お肉のテカリ度合い
	ConstantBuffer* pMeatSpecular = CreateObj<ConstantBuffer>("MeatSpecularCB");
	pMeatSpecular->Create(sizeof(XMFLOAT4));
	meatObj->m_specularCB = pMeatSpecular;

	// ポイントライトの設定
	ConstantBuffer* pPointLightCB = CreateObj<ConstantBuffer>("PointLightCB");
	pPointLightCB->Create(sizeof(XMFLOAT4) * 3);
	ptLightObj->m_lightCB = pPointLightCB;

	// ポイントライトの設定(オブジェクト)
	ConstantBuffer* pPtLightCB = CreateObj<ConstantBuffer>("ptLightCB");
	pPtLightCB->Create(sizeof(XMFLOAT4) * 3);
	
	// ゲーム内時間の設定
	ConstantBuffer* pTimerCB = CreateObj<ConstantBuffer>("timeCB");
	pTimerCB->Create(sizeof(XMFLOAT4));

	// リムライトのバッファ
	ConstantBuffer* plimlightCB = CreateObj<ConstantBuffer>("limlightCB");
	plimlightCB->Create(sizeof(XMFLOAT4));
	
	// 被写界深度の設定
	// 板ポリゴンの初期化
	g_pGeometory = new CGeometory();
	g_pGeometory->InitPolygonVertex();
	m_cameraDepth = new CameraDepth(m_pSceneModel[OBJ_SCREEN]);
	#pragma endregion

	// ライトの初期値を格納
	pDefLight = GetObj<ConstantBuffer>("CBLight");
	#pragma endregion 

	// ライトの初期化
	InitLight();
}
#pragma endregion 
void SceneBaseShader::Uninit()
{
	CSound::Stop(BGM_COOKING);
	CSound::Stop(BGM_ENVIRONMENT);
	g_pGeometory->UninitGeometory();
}
void SceneBaseShader::Update()
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pCamBuf = GetObj<ConstantBuffer>("CameraCB");
	CameraDCC* pCameraDCC = GetObj<CameraDCC>("CameraDCC");
	//ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");

	// タイマーの更新
	++m_time;

	skyObj->Update();

	meatObj->Update();
	m_PointLight->Update();

	playerObj->Update(m_pSceneModel[OBJ_TABLE]->m_vertex.pos);
	//ImGui::Text("playerPos: %.3f, %.3f, %.3f", playerObj->m_vertex.pos.x, playerObj->m_vertex.pos.y, playerObj->m_vertex.pos.z);

	if (IsKeyTrigger(VK_F1))
	{
		isDebug = !isDebug;
		pCameraDCC->canMove = isDebug;
	}
	//m_pSceneModel[OBJ_SKYDOME]->m_vertex.rot.y += 0.003f;

	// シェーダー設定
	SetDirectionLight();			// ディレクションライト
}
#pragma region draw
void SceneBaseShader::Draw()
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("LightCB");
	ConstantBuffer* pCamBuf = GetObj<ConstantBuffer>("CameraCB");
	ConstantBuffer* pPointLight = GetObj<ConstantBuffer>("PointLightCB");
	ConstantBuffer* pPtLight = GetObj<ConstantBuffer>("ptLightCB");
	ConstantBuffer* pTime = GetObj<ConstantBuffer>("timeCB");
	ConstantBuffer* pMeatCondition = GetObj<ConstantBuffer>("MeatCB");
	ConstantBuffer* pLimlightCB = GetObj<ConstantBuffer>("limlightCB");

	// 頂点シェーダーの設定
	#pragma region set vertex shader
	XMFLOAT4X4 mat[3];
	XMMATRIX worldMatrix = XMMatrixIdentity();
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	#pragma endregion 

	Texture* pTex = nullptr;
	Texture* pCanvas;
	DepthStencil* pDepth;
	RenderTarget * pRTVCanvas;
	ID3D11RenderTargetView* pView;

	#pragma region screen 
	// 描画先の変更
	pCanvas = GetObj<Texture>("LuminanceCanvas");
	pDepth = GetObj<DepthStencil>("LuminanceCanvasDSV");

	pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	pView = pRTVCanvas->GetView();
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// 描画先のクリア
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GetContext()->ClearRenderTargetView(pView, color);
	GetContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	skyObj->SetCamera(pCamera, pWVP);
	skyObj->Draw();
	
	// フィールドの描画設定
	//DrawField(_worldMatrix, i, _mat[0], pWVP, pTex, pLight, pCamera, pCamBuf);
	for (int i = 0; i < OBJ_MAX; ++i)
	{
		// 回転
		worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.x));
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.y));
		worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.z));

		// 座標変換
		worldMatrix *= XMMatrixTranslation(m_pSceneModel[i]->m_vertex.pos.x, m_pSceneModel[i]->m_vertex.pos.y, m_pSceneModel[i]->m_vertex.pos.z);
		//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));

		pWVP->Write(&mat);
		pWVP->BindVS(0);

		if (i == OBJ_ROCK)
		{
			// 法線マップ設定
			pTex = GetObj<Texture>("RockNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		else if (i == OBJ_TENT)
		{
			// 法線マップ設定
			pTex = GetObj<Texture>("ClothNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}

		// ピクセルシェーダーの設定
		#pragma region set pixel shader
		if (m_isView[SHADER_DIRECTIONLIGHT])
		{
			XMFLOAT4 ptLightPos = { m_PointLight->m_pos.x, m_PointLight->m_pos.y,m_PointLight->m_pos.z, 0.0f };
			XMFLOAT4 objBuf[] = {
			m_directionLight.lightDir,
			m_directionLight.lightColor,
			ptLightPos,
			m_PointLight->m_color
			};
			pPtLight->Write(&objBuf);
			pPtLight->BindPS(0);
			SetCamera(pCamera, pCamBuf);	// カメラ
		}
		else
		{
			pLight->Write(pDefLight);
			pLight->BindPS(0);
		}
		/*if (i == OBJ_ROCK)
		{
			MeshBuffer** ppMeshes = GetObj<MeshBuffer*>("rockBump");
			for (int i = 0; i < m_pSceneModel[i]->GetMeshNum(); ++i)
			{
				ppMeshes[i]->Draw();
			}
		}
		else
		{
			m_pSceneModel[i]->Draw();
		}*/
		m_pSceneModel[i]->Draw();
	}
	#pragma endregion 
#pragma endregion 

	// フィールドの描画
	#pragma region draw field
	// 描画先を元の画面に戻す
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	for (int i = 0; i < OBJ_MAX; ++i)
	{
		// 座標変換
		#pragma region set world matrix
		// 回転
		worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.x));
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.y));
		worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.z));

		// 座標変換
		worldMatrix *= XMMatrixTranslation(m_pSceneModel[i]->m_vertex.pos.x, m_pSceneModel[i]->m_vertex.pos.y, m_pSceneModel[i]->m_vertex.pos.z);
		//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));

		pWVP->Write(&mat);
		pWVP->BindVS(0);

		if (i == OBJ_ROCK)
		{
			// 法線マップ設定
			pTex = GetObj<Texture>("RockNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		else if (i == OBJ_TENT)
		{
			// 法線マップ設定
			pTex = GetObj<Texture>("ClothNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		#pragma endregion 

		// ピクセルシェーダーの設定
		#pragma region set pixel shader
		if (m_isView[SHADER_DIRECTIONLIGHT])
		{
			/*XMFLOAT4 objBuf[] = {
				m_directionLight.lightDir,
				m_directionLight.lightColor,
				XMFLOAT4(m_objSetting.smooth, 0.0f, 0.0f, 0.0f) ,
			};
			pLight->Write(&objBuf);
			pLight->BindPS(0);*/

			// オブジェクトが登録されていない場合次のオブジェクトに移行する
			if (m_pSceneModel[i] == nullptr) continue;
			
			if (i == OBJ_FIRE)
			{
				XMFLOAT4 fTime = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&fTime);
				pTime->BindPS(0);
			}
			else if (i == OBJ_MEAT)
			{
				// 肉の焼き具合の設定
				/*XMFLOAT4 condition = { meatObj->m_condition, meatObj->m_condition, 0.0f, 0.0f };
				pMeatCondition->Write(&condition);
				pMeatCondition->BindPS(2);*/

				// 拡散光の設定
				XMFLOAT4 objBuf[] = {
				m_directionLight.lightDir,
				m_directionLight.lightColor,
				XMFLOAT4(m_pSceneModel[i]->m_smooth, 0.0f, 0.0f, 0.0f) ,
				};
				pLight->Write(&objBuf);
				pLight->BindPS(0);
				
				// ポイントライトの設定
				XMFLOAT4 pointLightPos = { m_PointLight->m_pos.x, m_PointLight->m_pos.y,m_PointLight->m_pos.z,0.0f };
				XMFLOAT4 pointLightRange = { m_PointLight->m_range, 0.0f, 0.0f, 0.0f };
				XMFLOAT4 ptBuf[] = {
				pointLightPos,
				m_PointLight->m_color,
				pointLightRange,
				};
				pPointLight->Write(&ptBuf);
				pPointLight->BindPS(4);

				// ポイントライトのアニメーションの設定
				XMFLOAT4 time = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&time);
				pTime->BindPS(5);
				GetPS(PS_MEAT)->Bind();
			}
			else
			{
				// 拡散光の設定
				XMFLOAT4 objBuf[] = {
				m_directionLight.lightDir,
				m_directionLight.lightColor,
				XMFLOAT4(m_pSceneModel[i]->m_smooth, 0.0f, 0.0f, 0.0f) ,
				};
				pLight->Write(&objBuf);
				pLight->BindPS(0);

				// ポイントライトの設定
				XMFLOAT4 pointLightPos = { m_PointLight->m_pos.x, m_PointLight->m_pos.y,m_PointLight->m_pos.z,0.0f };
				XMFLOAT4 pointLightRange = { m_PointLight->m_range, 0.0f, 0.0f, 0.0f };
				XMFLOAT4 ptBuf[] = {
				pointLightPos,
				m_PointLight->m_color,
				pointLightRange,
				};
				pPtLight->Write(&ptBuf);
				pPtLight->BindPS(4);

				// ポイントライトのアニメーションの設定
				XMFLOAT4 time = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&time);
				pTime->BindPS(5);
				GetPS(PS_OBJECT)->Bind();
				GetPS(PS_PLAYER)->Bind();
			}

			// 回復中はリムライトを設定する
			if (i == OBJ_PLAYER)
			{
				XMFLOAT4 limBuf;
				if (playerObj->m_isHealPoint)
				{
					limBuf = { 1.0f, 0.0f, 0.0f, 0.0f };
				}
				else
				{
					limBuf = { 0.0f, 0.0f, 0.0f, 0.0f };
				}
				pLimlightCB->Write(&limBuf);
				pLimlightCB->BindPS(6);
				GetPS(PS_PLAYER)->Bind();
			}
			
			SetCamera(pCamera, pCamBuf);	// カメラ
		}
		else
		{
			pLight->Write(pDefLight);
			pLight->BindPS(0);
		}

		if (i == OBJ_SCREEN)
			SetTexturePS(pCanvas->GetResource(), 1);

		if (i == OBJ_POINTLIGHT && !isDebug)
		{
			return;
		}
		m_pSceneModel[i]->Draw();
		
		#pragma endregion 
	}

	// 板ポリ描画
	/*CGeometory::DrawPolygon(XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(200.0f, 100.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f));*/
	//pFoodManager->Draw(pWVP, mat);
	#pragma endregion 
}
#pragma endregion 

// モデルの初期化
void SceneBaseShader::InitModel()
{
	// ティーポットモデルの設定
	#pragma region set teapot model
	Model* pModel;
	pModel = CreateObj<Model>("TeapotModel");
	pModel->Load("Assets/Model/teapotSmooth.fbx", 0.05f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 1.0f, 0.5f, 0.0f };
	m_pSceneModel[OBJ_TEAPOT] = pModel;
	m_isView[SHADER_OBJECT] = true;
#pragma endregion 

	// ライトモデルの設定
	#pragma region set light model
	pModel = CreateObj<Model>("pointLightModel");
	ptLightObj = new PointLight(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	m_pSceneModel[OBJ_POINTLIGHT] = pModel;
	m_PointLight = ptLightObj;
#pragma endregion

// 山モデルの生成
#pragma region mountain
	pModel = CreateObj<Model>("BGModel");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 10.0f, -0.5f, 5.0f };
	pModel->m_vertex.rot.y = 90.0f;
	m_pSceneModel[OBJ_BG] = pModel;
#pragma endregion 

	// 空の生成
#pragma region sky
	Model* pSkyModel[SKY_MAX] = {
		GetObj<Model>("SkyDome_noon"),
		GetObj<Model>("SkyDome_evening"),
		GetObj<Model>("SkyDome_night"),
	};
	skyObj = new CSkyDome(pSkyModel);
	//m_pSceneModel[OBJ_SKYDOME] = pModel;
#pragma endregion

	// 岩モデルの生成
#pragma region rock
	pModel = CreateObj<Model>("RockModel");
	rockObj = new CRock(pModel, XMFLOAT3(-5.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_ROCK] = pModel;
	MeshBuffer** ppMesh = Create_BumpModel(pModel);
	RegisterObj<MeshBuffer*>("rockBump", ppMesh);
	m_pBumpModel[BUMPMODEL_ROCK] = ppMesh;

	// 法線マップの作成
	Texture* pTex = TextureFactory::CreateFromFile("Assets/Texture/normal_rock.png");
	RegisterObj<Texture>("RockNormalMap", pTex);
#pragma endregion 

	// 地面モデルの生成
#pragma region ground
	pModel = CreateObj<Model>("MountainModel");
	mountainObj = new CMountain(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_MOUNTAIN] = pModel;
#pragma endregion 

	// 焚火モデルの生成
#pragma region bonfire
	pModel = CreateObj<Model>("BonfireModel");
	bonfireObj = new CBonfire(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_BONFIRE] = pModel;
#pragma endregion 

	// 火のモデル生成
#pragma region set fire model
	pModel = CreateObj<Model>("fireModel");
	fireObj = new Cfire(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_FIRE] = pModel;

	// 輝度抽出用レンダーターゲットの作成
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 1280.0f, 720.0f);
	RegisterObj<Texture>("LuminanceCanvas", pCanvas);
	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(
		TextureFactory::CreateDepthStencil(1280, 720, true)
		);
	RegisterObj<DepthStencil>("LuminanceCanvasDSV", pDepth);
	MeshBuffer::Description desc = {};

	// 輝度抽出用スプライトの作成
	/*pModel = CreateObj<Model>("LuminanceSprite");
	fireObj->SetSprite(pModel);*/
	//m_pSceneModel[OBJ_SCREEN] = pModel;
#pragma endregion 

// テントモデルの生成
#pragma region tent
	pModel = CreateObj<Model>("tentModel");
	tentObj = new Ctent(pModel, XMFLOAT3(-5.0f, 0.0f, 5.0f));
	pModel->m_vertex.rot.y = -90.0f;
	m_pSceneModel[OBJ_TENT] = pModel;
	/*ppMesh = Create_BumpModel(pModel);
	RegisterObj<MeshBuffer*>("tentBump", ppMesh);
	m_pBumpModel[BUMPMODEL_TENT] = ppMesh;*/
#pragma endregion 

	// イスモデルの生成
#pragma region chair
	pModel = CreateObj<Model>("chairModel");
	chairObj = new CChair(pModel, XMFLOAT3(-0.8f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_CHAIR] = pModel;
#pragma endregion 

	// プレイヤーの生成
#pragma region player
	pModel = CreateObj<Model>("playerModel");
	playerObj = new CPlayer(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	pModel->m_smooth = 100.0f;		// 鏡面反射光の強さの設定
	// カメラの設定
	CameraDCC* pCameraDCC = GetObj<CameraDCC>("CameraDCC");
	playerObj->m_camera = pCameraDCC;
	m_pSceneModel[OBJ_PLAYER] = pModel;
#pragma endregion 

	// テーブルモデルの生成
#pragma region table
	pModel = CreateObj<Model>("tableModel");
	pModel->Load("Assets/Model/model_table.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 5.0f, 0.5f, 1.0f };
	pModel->m_vertex.rot.y = 90.0f;

	// ショップの生成
	CShop* pShop = new CShop(pModel);
	m_pSceneModel[OBJ_TABLE] = pModel;

	pFoodManager = new CFood();
	m_pSceneModel[OBJ_FOOD] = pFoodManager->Init();
	
#pragma endregion 

	// 木のモデルの生成
	/*pModel = CreateObj<Model>("treeModel");
	treeObj = new CTree(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_TREE] = pModel;*/

	// 輝度抽出用スプライトの作成
	pModel = CreateObj<Model>("screenModel");
	pModel->Load("Assets/Model/model_screen.fbx", 1.0f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_SCREEN));
	pModel->m_vertex.pos.y = 3.0f;
	m_pSceneModel[OBJ_SCREEN] = pModel;

	// 法線マップの作成
	pTex = TextureFactory::CreateFromFile("Assets/Texture/normal_cloth.png");
	RegisterObj<Texture>("ClothNormalMap", pTex);

	// 肉モデルの作成
	pModel = CreateObj<Model>("meatModel");
	meatObj = new Cmeat(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	m_pSceneModel[OBJ_MEAT] = pModel;

	// 肉の骨モデルの作成
	pModel = CreateObj<Model>("meatBoneModel");
	pModel->Load("Assets/Model/model_meat_bone.fbx", 0.3f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 0.0f, 0.5f, 0.0f };
	meatObj->m_pBoneModel = pModel;
	m_pSceneModel[OBJ_MEATBONE] = pModel;

#pragma endregion 

	// 店モデルの設定
	#pragma region shop model
	
	#pragma endregion 
}

// ライトの初期化
#pragma region init light
void SceneBaseShader::InitLight()
{
	// ライトの方向の設定
	m_directionLight.lightDir = { 1.0f, 1.0f, -1.0f, 1.0f };
	// ライトのカラーの設定
	m_directionLight.lightColor = { 1.0f, 0.0f, 0.0f, 1.0f };
}
#pragma endregion 

// 拡散光の設定
#pragma region set direction light
void SceneBaseShader::SetDirectionLight()
{
	bOldImgFlg = m_ImGUIFlg[CB_DIRTIONLIGHT];
	// スイッチ
	ImGui::Checkbox("DIRECTIONLIGHT", &m_ImGUIFlg[CB_DIRTIONLIGHT]);
	if (m_ImGUIFlg[CB_DIRTIONLIGHT] != bOldImgFlg)
	{
		// ディレクションライトを付ける
		m_isView[SHADER_DIRECTIONLIGHT] = !m_isView[SHADER_DIRECTIONLIGHT];
	}

	// 数値入力
	static float sliderPos[3] = { 1.0f, -1.0f, 1.0f };
	ImGui::SliderFloat3("pos", sliderPos, -1.0f, 1.0f);
	m_directionLight.lightDir = { sliderPos[0], -sliderPos[1], sliderPos[2], 0.0f};
	static float sliderColor[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::SliderFloat3("color", sliderColor, 0.0f, 1.0f);
	m_directionLight.lightColor = { sliderColor[0], sliderColor[1], sliderColor[2], 1.0f };
}
#pragma endregion 

// 鏡面反射光の設定
#pragma region set specular
//void SceneBaseShader::SetSpecular()
//{
//	// 絞り値入力
//	static float value = 1.0f;
//	ImGui::SliderFloat("smooth", &value, 1.0f, 200.0f);
//	m_pSceneModel[i]-> = value;
//}
#pragma endregion 

// カメラの設定
#pragma region set camera
void SceneBaseShader::SetCamera(CameraBase* _pCamera, ConstantBuffer* _pCameraBuf)
{
	XMFLOAT4 camPos = { _pCamera->GetPos().x, _pCamera->GetPos().y, _pCamera->GetPos().z, 0.0f};
	_pCameraBuf->Write(&camPos);
	_pCameraBuf->BindPS(1);
}
#pragma endregion 

// 拡散光の描画
#pragma region draw direction light
void SceneBaseShader::DrawDirectionLight(ConstantBuffer* _pLight)
{
	_pLight->Write(&m_directionLight);
	_pLight->BindPS(0);
}
#pragma endregion 

// 鏡面反射光の描画
#pragma region draw specular
//void SceneBaseShader::DrawSpecular(ConstantBuffer* _pLight)
//{
//	_pLight->Write(&m_objSetting.smooth);
//	_pLight->BindPS(0);
//}
#pragma endregion 

// バンプモデルの作成
#pragma region create bump model
MeshBuffer** Create_BumpModel(Model* _pModel)
{
	//バンプ用のモデル生成
	int meshNum = _pModel->GetMeshNum();
	MeshBuffer** _ppMesh = new MeshBuffer * [meshNum];
	for (int i = 0; i < meshNum; ++i)
	{
		MeshBuffer::Description desc = {};
		//メッシュの蝶点数に合わせて、バンプの頂点を確保
		int vtxNum = _pModel->GetMesh(i)->vertexNum;
		Model::Vertex* pVtx = _pModel->GetMesh(i)->pVertices;
		BumpVertex* pBumpVtx = new BumpVertex[vtxNum];
		for (int j = 0; j < vtxNum; ++j)
		{
			//バンプに頂点をコピー
			pBumpVtx[j].pos = pVtx[j].pos;
			pBumpVtx[j].normal = pVtx[j].normal;
			pBumpVtx[j].uv = pVtx[j].uv;
		}
		//面数に合わせて、接ベクトルを計算
		int idxNum = _pModel->GetMesh(i)->indexNum;
		unsigned int* pIdx = _pModel->GetMesh(i)->pIndices;
		int* tangentVtxCnt = new int[vtxNum];	//一つの頂点が複数の面に利用されている場合
		ZeroMemory(tangentVtxCnt, sizeof(int) * vtxNum);
		for (int j = 0; j < idxNum; j += 3)
		{
			//面を構成するインデックス
			int idx0 = pIdx[j + 0];
			int idx1 = pIdx[j + 1];
			int idx2 = pIdx[j + 2];
			//接ベクトル情報の格納先
			DirectX::XMFLOAT3* pTangents[] =
			{
				&pBumpVtx[idx0].tangent,
				&pBumpVtx[idx1].tangent,
				&pBumpVtx[idx2].tangent
			};
			//接ベクトルの計算に必要な各種じょうほう
			DirectX::XMFLOAT3 v0(
				pBumpVtx[idx1].pos.x - pBumpVtx[idx0].pos.x,
				pBumpVtx[idx1].pos.y - pBumpVtx[idx0].pos.y,
				pBumpVtx[idx1].pos.z - pBumpVtx[idx0].pos.z
			);
			DirectX::XMFLOAT3 v1(
				pBumpVtx[idx2].pos.x - pBumpVtx[idx0].pos.x,
				pBumpVtx[idx2].pos.y - pBumpVtx[idx0].pos.y,
				pBumpVtx[idx2].pos.z - pBumpVtx[idx0].pos.z
			);
			DirectX::XMFLOAT2 t0(
				pBumpVtx[idx1].uv.x - pBumpVtx[idx0].uv.x,
				pBumpVtx[idx1].uv.y - pBumpVtx[idx0].uv.y
			);
			DirectX::XMFLOAT2 t1(
				pBumpVtx[idx2].uv.x - pBumpVtx[idx0].uv.x,
				pBumpVtx[idx2].uv.y - pBumpVtx[idx0].uv.y
			);
			/*
			|T.x T.y T.z|				1            |
			|           | = -------------------------|
			|B.x B.y B.z|   t0.x * t1.y - t1.x * t0.y|
			*/
			float t = 1.0f / (t0.x * t1.y - t1.x * t0.y);
			DirectX::XMVECTOR T = DirectX::XMVectorSet(
				t * (t1.y * v0.x - t0.y * v1.x),
				t * (t1.y * v0.y - t0.y * v1.y),
				t * (t1.y * v0.z - t0.y * v1.z), 0.0f
			);
			T = DirectX::XMVector3Normalize(T);
			//接ベクトルは合算して最後に平均を出す
			for (int k = 0; k < 3; ++k)
			{
				DirectX::XMStoreFloat3(pTangents[k],
					DirectX::XMVectorAdd(T, DirectX::XMLoadFloat3(pTangents[k])));
				tangentVtxCnt[pIdx[j + k]]++;
			}
		}
		//接ベクトルの平均を算出
		for (int j = 0; j < vtxNum; ++j)
		{
			pBumpVtx[j].tangent.x /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.y /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.z /= tangentVtxCnt[j];
		}
		//モデルデータ作成
		desc.pVtx = pBumpVtx;
		desc.vtxSize = sizeof(BumpVertex);
		desc.vtxCount = vtxNum;
		desc.pIdx = pIdx;
		desc.idxSize = sizeof(unsigned int);
		desc.idxCount = idxNum;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_ppMesh[i] = new MeshBuffer(desc);

		//不要なデータ削除
		delete[] pBumpVtx;
		delete[] tangentVtxCnt;
	}

	return _ppMesh;
}
#pragma endregion 
