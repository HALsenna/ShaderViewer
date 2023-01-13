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
	DirectX::XMFLOAT3 tangent;		// �ڃx�N�g��(�^���W�F���g�x�N�g��)
	// ���[���h��ԏ�Ńe�N�X�`����\��t���鉡�̕���
};

// �O���[�o���ϐ�
static ConstantBuffer* pDefLight;
static bool bOldImgFlg = false;
float fRot = 0.0f;
bool isCooking = false;
static CGeometory* g_pGeometory;

bool isDebug = true;
// �v���g�^�C�v�錾
MeshBuffer** Create_BumpModel(Model* _pModel);

#pragma region init
void SceneBaseShader::Init()
{
	// �^�C�}�[�̏�����
	m_time = 0.0f;

	// �V�F�[�_�[�\���t���O�̏�����
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

	// BGM�Đ�
	CSound::Play(BGM_ENVIRONMENT);

	// ���f���̐ݒ�
	#pragma region set model
	InitModel();
	#pragma endregion 

	// �萔�o�b�t�@�̐ݒ�
	#pragma region set constant buffer
	// �J�����̐���
	#pragma region set camera
	// �J����
	ConstantBuffer* pCamBuf = CreateObj<ConstantBuffer>("CameraCB");
	pCamBuf->Create(sizeof(DirectX::XMFLOAT4) * 2);
	// ���C�g�̐F
	ConstantBuffer* pLight = CreateObj<ConstantBuffer>("LightCB");
	pLight->Create(sizeof(XMFLOAT4) * 4);

	// �����̐F
	ConstantBuffer* pMeatCondition = CreateObj<ConstantBuffer>("MeatCB");
	pMeatCondition->Create(sizeof(XMFLOAT4));
	meatObj->m_cb = pMeatCondition;
	// �����̃e�J���x����
	ConstantBuffer* pMeatSpecular = CreateObj<ConstantBuffer>("MeatSpecularCB");
	pMeatSpecular->Create(sizeof(XMFLOAT4));
	meatObj->m_specularCB = pMeatSpecular;

	// �|�C���g���C�g�̐ݒ�
	ConstantBuffer* pPointLightCB = CreateObj<ConstantBuffer>("PointLightCB");
	pPointLightCB->Create(sizeof(XMFLOAT4) * 3);
	ptLightObj->m_lightCB = pPointLightCB;

	// �|�C���g���C�g�̐ݒ�(�I�u�W�F�N�g)
	ConstantBuffer* pPtLightCB = CreateObj<ConstantBuffer>("ptLightCB");
	pPtLightCB->Create(sizeof(XMFLOAT4) * 3);
	
	// �Q�[�������Ԃ̐ݒ�
	ConstantBuffer* pTimerCB = CreateObj<ConstantBuffer>("timeCB");
	pTimerCB->Create(sizeof(XMFLOAT4));

	// �������C�g�̃o�b�t�@
	ConstantBuffer* plimlightCB = CreateObj<ConstantBuffer>("limlightCB");
	plimlightCB->Create(sizeof(XMFLOAT4));
	
	// ��ʊE�[�x�̐ݒ�
	// �|���S���̏�����
	g_pGeometory = new CGeometory();
	g_pGeometory->InitPolygonVertex();
	m_cameraDepth = new CameraDepth(m_pSceneModel[OBJ_SCREEN]);
	#pragma endregion

	// ���C�g�̏����l���i�[
	pDefLight = GetObj<ConstantBuffer>("CBLight");
	#pragma endregion 

	// ���C�g�̏�����
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

	// �^�C�}�[�̍X�V
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

	// �V�F�[�_�[�ݒ�
	SetDirectionLight();			// �f�B���N�V�������C�g
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

	// ���_�V�F�[�_�[�̐ݒ�
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
	// �`���̕ύX
	pCanvas = GetObj<Texture>("LuminanceCanvas");
	pDepth = GetObj<DepthStencil>("LuminanceCanvasDSV");

	pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	pView = pRTVCanvas->GetView();
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// �`���̃N���A
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GetContext()->ClearRenderTargetView(pView, color);
	GetContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	skyObj->SetCamera(pCamera, pWVP);
	skyObj->Draw();
	
	// �t�B�[���h�̕`��ݒ�
	//DrawField(_worldMatrix, i, _mat[0], pWVP, pTex, pLight, pCamera, pCamBuf);
	for (int i = 0; i < OBJ_MAX; ++i)
	{
		// ��]
		worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.x));
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.y));
		worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.z));

		// ���W�ϊ�
		worldMatrix *= XMMatrixTranslation(m_pSceneModel[i]->m_vertex.pos.x, m_pSceneModel[i]->m_vertex.pos.y, m_pSceneModel[i]->m_vertex.pos.z);
		//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));

		pWVP->Write(&mat);
		pWVP->BindVS(0);

		if (i == OBJ_ROCK)
		{
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("RockNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		else if (i == OBJ_TENT)
		{
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("ClothNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}

		// �s�N�Z���V�F�[�_�[�̐ݒ�
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
			SetCamera(pCamera, pCamBuf);	// �J����
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

	// �t�B�[���h�̕`��
	#pragma region draw field
	// �`�������̉�ʂɖ߂�
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	for (int i = 0; i < OBJ_MAX; ++i)
	{
		// ���W�ϊ�
		#pragma region set world matrix
		// ��]
		worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.x));
		worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.y));
		worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_pSceneModel[i]->m_vertex.rot.z));

		// ���W�ϊ�
		worldMatrix *= XMMatrixTranslation(m_pSceneModel[i]->m_vertex.pos.x, m_pSceneModel[i]->m_vertex.pos.y, m_pSceneModel[i]->m_vertex.pos.z);
		//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
		XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));

		pWVP->Write(&mat);
		pWVP->BindVS(0);

		if (i == OBJ_ROCK)
		{
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("RockNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		else if (i == OBJ_TENT)
		{
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("ClothNormalMap");
			SetTexturePS(pTex->GetResource(), 1);
		}
		#pragma endregion 

		// �s�N�Z���V�F�[�_�[�̐ݒ�
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

			// �I�u�W�F�N�g���o�^����Ă��Ȃ��ꍇ���̃I�u�W�F�N�g�Ɉڍs����
			if (m_pSceneModel[i] == nullptr) continue;
			
			if (i == OBJ_FIRE)
			{
				XMFLOAT4 fTime = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&fTime);
				pTime->BindPS(0);
			}
			else if (i == OBJ_MEAT)
			{
				// ���̏Ă���̐ݒ�
				/*XMFLOAT4 condition = { meatObj->m_condition, meatObj->m_condition, 0.0f, 0.0f };
				pMeatCondition->Write(&condition);
				pMeatCondition->BindPS(2);*/

				// �g�U���̐ݒ�
				XMFLOAT4 objBuf[] = {
				m_directionLight.lightDir,
				m_directionLight.lightColor,
				XMFLOAT4(m_pSceneModel[i]->m_smooth, 0.0f, 0.0f, 0.0f) ,
				};
				pLight->Write(&objBuf);
				pLight->BindPS(0);
				
				// �|�C���g���C�g�̐ݒ�
				XMFLOAT4 pointLightPos = { m_PointLight->m_pos.x, m_PointLight->m_pos.y,m_PointLight->m_pos.z,0.0f };
				XMFLOAT4 pointLightRange = { m_PointLight->m_range, 0.0f, 0.0f, 0.0f };
				XMFLOAT4 ptBuf[] = {
				pointLightPos,
				m_PointLight->m_color,
				pointLightRange,
				};
				pPointLight->Write(&ptBuf);
				pPointLight->BindPS(4);

				// �|�C���g���C�g�̃A�j���[�V�����̐ݒ�
				XMFLOAT4 time = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&time);
				pTime->BindPS(5);
				GetPS(PS_MEAT)->Bind();
			}
			else
			{
				// �g�U���̐ݒ�
				XMFLOAT4 objBuf[] = {
				m_directionLight.lightDir,
				m_directionLight.lightColor,
				XMFLOAT4(m_pSceneModel[i]->m_smooth, 0.0f, 0.0f, 0.0f) ,
				};
				pLight->Write(&objBuf);
				pLight->BindPS(0);

				// �|�C���g���C�g�̐ݒ�
				XMFLOAT4 pointLightPos = { m_PointLight->m_pos.x, m_PointLight->m_pos.y,m_PointLight->m_pos.z,0.0f };
				XMFLOAT4 pointLightRange = { m_PointLight->m_range, 0.0f, 0.0f, 0.0f };
				XMFLOAT4 ptBuf[] = {
				pointLightPos,
				m_PointLight->m_color,
				pointLightRange,
				};
				pPtLight->Write(&ptBuf);
				pPtLight->BindPS(4);

				// �|�C���g���C�g�̃A�j���[�V�����̐ݒ�
				XMFLOAT4 time = { m_time, 0.0f, 0.0f, 0.0f };
				pTime->Write(&time);
				pTime->BindPS(5);
				GetPS(PS_OBJECT)->Bind();
				GetPS(PS_PLAYER)->Bind();
			}

			// �񕜒��̓������C�g��ݒ肷��
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
			
			SetCamera(pCamera, pCamBuf);	// �J����
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

	// �|���`��
	/*CGeometory::DrawPolygon(XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(200.0f, 100.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f));*/
	//pFoodManager->Draw(pWVP, mat);
	#pragma endregion 
}
#pragma endregion 

// ���f���̏�����
void SceneBaseShader::InitModel()
{
	// �e�B�[�|�b�g���f���̐ݒ�
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

	// ���C�g���f���̐ݒ�
	#pragma region set light model
	pModel = CreateObj<Model>("pointLightModel");
	ptLightObj = new PointLight(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	m_pSceneModel[OBJ_POINTLIGHT] = pModel;
	m_PointLight = ptLightObj;
#pragma endregion

// �R���f���̐���
#pragma region mountain
	pModel = CreateObj<Model>("BGModel");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 10.0f, -0.5f, 5.0f };
	pModel->m_vertex.rot.y = 90.0f;
	m_pSceneModel[OBJ_BG] = pModel;
#pragma endregion 

	// ��̐���
#pragma region sky
	Model* pSkyModel[SKY_MAX] = {
		GetObj<Model>("SkyDome_noon"),
		GetObj<Model>("SkyDome_evening"),
		GetObj<Model>("SkyDome_night"),
	};
	skyObj = new CSkyDome(pSkyModel);
	//m_pSceneModel[OBJ_SKYDOME] = pModel;
#pragma endregion

	// �⃂�f���̐���
#pragma region rock
	pModel = CreateObj<Model>("RockModel");
	rockObj = new CRock(pModel, XMFLOAT3(-5.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_ROCK] = pModel;
	MeshBuffer** ppMesh = Create_BumpModel(pModel);
	RegisterObj<MeshBuffer*>("rockBump", ppMesh);
	m_pBumpModel[BUMPMODEL_ROCK] = ppMesh;

	// �@���}�b�v�̍쐬
	Texture* pTex = TextureFactory::CreateFromFile("Assets/Texture/normal_rock.png");
	RegisterObj<Texture>("RockNormalMap", pTex);
#pragma endregion 

	// �n�ʃ��f���̐���
#pragma region ground
	pModel = CreateObj<Model>("MountainModel");
	mountainObj = new CMountain(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_MOUNTAIN] = pModel;
#pragma endregion 

	// ���΃��f���̐���
#pragma region bonfire
	pModel = CreateObj<Model>("BonfireModel");
	bonfireObj = new CBonfire(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_BONFIRE] = pModel;
#pragma endregion 

	// �΂̃��f������
#pragma region set fire model
	pModel = CreateObj<Model>("fireModel");
	fireObj = new Cfire(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_FIRE] = pModel;

	// �P�x���o�p�����_�[�^�[�Q�b�g�̍쐬
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, 1280.0f, 720.0f);
	RegisterObj<Texture>("LuminanceCanvas", pCanvas);
	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(
		TextureFactory::CreateDepthStencil(1280, 720, true)
		);
	RegisterObj<DepthStencil>("LuminanceCanvasDSV", pDepth);
	MeshBuffer::Description desc = {};

	// �P�x���o�p�X�v���C�g�̍쐬
	/*pModel = CreateObj<Model>("LuminanceSprite");
	fireObj->SetSprite(pModel);*/
	//m_pSceneModel[OBJ_SCREEN] = pModel;
#pragma endregion 

// �e���g���f���̐���
#pragma region tent
	pModel = CreateObj<Model>("tentModel");
	tentObj = new Ctent(pModel, XMFLOAT3(-5.0f, 0.0f, 5.0f));
	pModel->m_vertex.rot.y = -90.0f;
	m_pSceneModel[OBJ_TENT] = pModel;
	/*ppMesh = Create_BumpModel(pModel);
	RegisterObj<MeshBuffer*>("tentBump", ppMesh);
	m_pBumpModel[BUMPMODEL_TENT] = ppMesh;*/
#pragma endregion 

	// �C�X���f���̐���
#pragma region chair
	pModel = CreateObj<Model>("chairModel");
	chairObj = new CChair(pModel, XMFLOAT3(-0.8f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_CHAIR] = pModel;
#pragma endregion 

	// �v���C���[�̐���
#pragma region player
	pModel = CreateObj<Model>("playerModel");
	playerObj = new CPlayer(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	pModel->m_smooth = 100.0f;		// ���ʔ��ˌ��̋����̐ݒ�
	// �J�����̐ݒ�
	CameraDCC* pCameraDCC = GetObj<CameraDCC>("CameraDCC");
	playerObj->m_camera = pCameraDCC;
	m_pSceneModel[OBJ_PLAYER] = pModel;
#pragma endregion 

	// �e�[�u�����f���̐���
#pragma region table
	pModel = CreateObj<Model>("tableModel");
	pModel->Load("Assets/Model/model_table.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 5.0f, 0.5f, 1.0f };
	pModel->m_vertex.rot.y = 90.0f;

	// �V���b�v�̐���
	CShop* pShop = new CShop(pModel);
	m_pSceneModel[OBJ_TABLE] = pModel;

	pFoodManager = new CFood();
	m_pSceneModel[OBJ_FOOD] = pFoodManager->Init();
	
#pragma endregion 

	// �؂̃��f���̐���
	/*pModel = CreateObj<Model>("treeModel");
	treeObj = new CTree(pModel, XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pSceneModel[OBJ_TREE] = pModel;*/

	// �P�x���o�p�X�v���C�g�̍쐬
	pModel = CreateObj<Model>("screenModel");
	pModel->Load("Assets/Model/model_screen.fbx", 1.0f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_SCREEN));
	pModel->m_vertex.pos.y = 3.0f;
	m_pSceneModel[OBJ_SCREEN] = pModel;

	// �@���}�b�v�̍쐬
	pTex = TextureFactory::CreateFromFile("Assets/Texture/normal_cloth.png");
	RegisterObj<Texture>("ClothNormalMap", pTex);

	// �����f���̍쐬
	pModel = CreateObj<Model>("meatModel");
	meatObj = new Cmeat(pModel, XMFLOAT3(0.0f, 0.5f, 0.0f));
	m_pSceneModel[OBJ_MEAT] = pModel;

	// ���̍����f���̍쐬
	pModel = CreateObj<Model>("meatBoneModel");
	pModel->Load("Assets/Model/model_meat_bone.fbx", 0.3f);
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_OBJECT));
	pModel->m_vertex.pos = { 0.0f, 0.5f, 0.0f };
	meatObj->m_pBoneModel = pModel;
	m_pSceneModel[OBJ_MEATBONE] = pModel;

#pragma endregion 

	// �X���f���̐ݒ�
	#pragma region shop model
	
	#pragma endregion 
}

// ���C�g�̏�����
#pragma region init light
void SceneBaseShader::InitLight()
{
	// ���C�g�̕����̐ݒ�
	m_directionLight.lightDir = { 1.0f, 1.0f, -1.0f, 1.0f };
	// ���C�g�̃J���[�̐ݒ�
	m_directionLight.lightColor = { 1.0f, 0.0f, 0.0f, 1.0f };
}
#pragma endregion 

// �g�U���̐ݒ�
#pragma region set direction light
void SceneBaseShader::SetDirectionLight()
{
	bOldImgFlg = m_ImGUIFlg[CB_DIRTIONLIGHT];
	// �X�C�b�`
	ImGui::Checkbox("DIRECTIONLIGHT", &m_ImGUIFlg[CB_DIRTIONLIGHT]);
	if (m_ImGUIFlg[CB_DIRTIONLIGHT] != bOldImgFlg)
	{
		// �f�B���N�V�������C�g��t����
		m_isView[SHADER_DIRECTIONLIGHT] = !m_isView[SHADER_DIRECTIONLIGHT];
	}

	// ���l����
	static float sliderPos[3] = { 1.0f, -1.0f, 1.0f };
	ImGui::SliderFloat3("pos", sliderPos, -1.0f, 1.0f);
	m_directionLight.lightDir = { sliderPos[0], -sliderPos[1], sliderPos[2], 0.0f};
	static float sliderColor[3] = { 0.0f, 0.0f, 0.0f };
	ImGui::SliderFloat3("color", sliderColor, 0.0f, 1.0f);
	m_directionLight.lightColor = { sliderColor[0], sliderColor[1], sliderColor[2], 1.0f };
}
#pragma endregion 

// ���ʔ��ˌ��̐ݒ�
#pragma region set specular
//void SceneBaseShader::SetSpecular()
//{
//	// �i��l����
//	static float value = 1.0f;
//	ImGui::SliderFloat("smooth", &value, 1.0f, 200.0f);
//	m_pSceneModel[i]-> = value;
//}
#pragma endregion 

// �J�����̐ݒ�
#pragma region set camera
void SceneBaseShader::SetCamera(CameraBase* _pCamera, ConstantBuffer* _pCameraBuf)
{
	XMFLOAT4 camPos = { _pCamera->GetPos().x, _pCamera->GetPos().y, _pCamera->GetPos().z, 0.0f};
	_pCameraBuf->Write(&camPos);
	_pCameraBuf->BindPS(1);
}
#pragma endregion 

// �g�U���̕`��
#pragma region draw direction light
void SceneBaseShader::DrawDirectionLight(ConstantBuffer* _pLight)
{
	_pLight->Write(&m_directionLight);
	_pLight->BindPS(0);
}
#pragma endregion 

// ���ʔ��ˌ��̕`��
#pragma region draw specular
//void SceneBaseShader::DrawSpecular(ConstantBuffer* _pLight)
//{
//	_pLight->Write(&m_objSetting.smooth);
//	_pLight->BindPS(0);
//}
#pragma endregion 

// �o���v���f���̍쐬
#pragma region create bump model
MeshBuffer** Create_BumpModel(Model* _pModel)
{
	//�o���v�p�̃��f������
	int meshNum = _pModel->GetMeshNum();
	MeshBuffer** _ppMesh = new MeshBuffer * [meshNum];
	for (int i = 0; i < meshNum; ++i)
	{
		MeshBuffer::Description desc = {};
		//���b�V���̒��_���ɍ��킹�āA�o���v�̒��_���m��
		int vtxNum = _pModel->GetMesh(i)->vertexNum;
		Model::Vertex* pVtx = _pModel->GetMesh(i)->pVertices;
		BumpVertex* pBumpVtx = new BumpVertex[vtxNum];
		for (int j = 0; j < vtxNum; ++j)
		{
			//�o���v�ɒ��_���R�s�[
			pBumpVtx[j].pos = pVtx[j].pos;
			pBumpVtx[j].normal = pVtx[j].normal;
			pBumpVtx[j].uv = pVtx[j].uv;
		}
		//�ʐ��ɍ��킹�āA�ڃx�N�g�����v�Z
		int idxNum = _pModel->GetMesh(i)->indexNum;
		unsigned int* pIdx = _pModel->GetMesh(i)->pIndices;
		int* tangentVtxCnt = new int[vtxNum];	//��̒��_�������̖ʂɗ��p����Ă���ꍇ
		ZeroMemory(tangentVtxCnt, sizeof(int) * vtxNum);
		for (int j = 0; j < idxNum; j += 3)
		{
			//�ʂ��\������C���f�b�N�X
			int idx0 = pIdx[j + 0];
			int idx1 = pIdx[j + 1];
			int idx2 = pIdx[j + 2];
			//�ڃx�N�g�����̊i�[��
			DirectX::XMFLOAT3* pTangents[] =
			{
				&pBumpVtx[idx0].tangent,
				&pBumpVtx[idx1].tangent,
				&pBumpVtx[idx2].tangent
			};
			//�ڃx�N�g���̌v�Z�ɕK�v�Ȋe�킶�傤�ق�
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
			//�ڃx�N�g���͍��Z���čŌ�ɕ��ς��o��
			for (int k = 0; k < 3; ++k)
			{
				DirectX::XMStoreFloat3(pTangents[k],
					DirectX::XMVectorAdd(T, DirectX::XMLoadFloat3(pTangents[k])));
				tangentVtxCnt[pIdx[j + k]]++;
			}
		}
		//�ڃx�N�g���̕��ς��Z�o
		for (int j = 0; j < vtxNum; ++j)
		{
			pBumpVtx[j].tangent.x /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.y /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.z /= tangentVtxCnt[j];
		}
		//���f���f�[�^�쐬
		desc.pVtx = pBumpVtx;
		desc.vtxSize = sizeof(BumpVertex);
		desc.vtxCount = vtxNum;
		desc.pIdx = pIdx;
		desc.idxSize = sizeof(unsigned int);
		desc.idxCount = idxNum;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_ppMesh[i] = new MeshBuffer(desc);

		//�s�v�ȃf�[�^�폜
		delete[] pBumpVtx;
		delete[] tangentVtxCnt;
	}

	return _ppMesh;
}
#pragma endregion 
