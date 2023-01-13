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
	"Assets/Texture/Title/TEX_TitleLogoBG.png",	// �^�C�g�����S(�w�i)
	"Assets/Texture/Title/TEX_Title.png",		// �^�C�g�����S(���O)
	"Assets/Texture/Title/TEX_TitlePressKey.png",	// �L�[����
	//"Assets/Texture/Title/TEX_Bane.png",	
};
const char* szDissolveTex = "Assets/Texture/dissolve.png";

const XMFLOAT3 TitleUIPos[TITLEUI_MAX]
{
	{0.0f, 50.0f, 0.0f},			// �^�C�g�����S(�w�i)
	{0.0f, 50.0f, 0.0f},			// �^�C�g�����S(���O)
	{0.0f, -100.0f, 0.0f},			// �L�[����
	//{0.0f, -100.0f, 0.0f},			
};

const XMFLOAT2 TitleUISize[TITLEUI_MAX]
{
	{0.5f, 0.5f},	// �^�C�g�����S
	{0.8f, 0.8f},	// �^�C�g�����S
	{0.3f, 0.3f},	// �L�[����
	//{0.5f, 0.5f},
};

const XMFLOAT4 UIColor[TITLEUI_MAX]
{
	{1.0f, 1.0f, 1.0f, 1.0f},		// �^�C�g�����S(�w�i)
	{0.0f, 0.0f, 0.0f, 0.0f},		// �^�C�g�����S(���O)
	{0.0f, 0.0f, 0.0f, 0.0f},		// �L�[����
	//{1.0f, 1.0f, 1.0f, 1.0f},		// �^�C�g�����S(�w�i)
};

// �O���[�o���ϐ�
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

// �v���g�^�C�v�錾
void SetVertexShader(ConstantBuffer* cb, CameraBase* camera, Model* model);
CUI** CreateUI();
void Bane();
//void StagingTitleLogo();
	                       
void SceneTitle::Init()
{
	m_pTitleScene = this;
	// �J�������Œ肷��
	CameraDCC* pCameraDCC = GetObj<CameraDCC>("CameraDCC");

	// �V�[���t�F�[�h�̐ݒ�
	g_pSceneFade = new SceneFade();
	g_pSceneFade->SetFade(SceneFade::FADE_IN, 1.0f);

	// ���C�g�̐ݒ�
	g_pLightManager = new CLight();

	// �I�u�W�F�N�g���̐ݒ�
	TitleObject obj;
	// �^�C�}�[�̐���
	g_pTimer = new CTimer();
	g_time = 0.0f;
	// �X�J�C�h�[��
	Model* pSkyModel[SKY_MAX] = {
		CreateObj<Model>("SkyDome_noon"),
		CreateObj<Model>("SkyDome_evening"),
		CreateObj<Model>("SkyDome_night"),
	};
	g_pSkydome = new CSkyDome(pSkyModel);

	// �w�i���f��
	obj = { "MountainModel", groundFile, XMFLOAT3(0.0f, -2.0f, -5.0f),
		XMFLOAT3(0.0f, 90.0f, 0.0f), 0.005f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// �����f��
	obj = { "BridgeModel", bridgeFile, XMFLOAT3(4.0f, -2.0f, 7.0f),
		XMFLOAT3(0.0f, 90.0f, 0.0f), 0.3f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// �ԃ��f��
	CFlower* flowerObj = new CFlower();
	obj = { "flowerpetalModel", flowerpetalFile, XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, VS_OBJECT, PS_FLOWER };
	SetObjInfo(obj);

	obj = { "flowerstemModel", flowerstemFile, XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, VS_OBJECT, PS_OBJECT };
	SetObjInfo(obj);

	// �Ԃ̐F�̒萔�o�b�t�@��ݒ肷��
	ConstantBuffer* pFlowerColorCB = CreateObj<ConstantBuffer>("CBFlowerColor");
	pFlowerColorCB->Create(sizeof(XMFLOAT4));

	// �������C�g�̐ݒ�
	/*ConstantBuffer* pDirectionLightCB = CreateObj<ConstantBuffer>("CBDirectionalLight");
	pDirectionLightCB->Create(sizeof(XMFLOAT4) * 2);*/

	ConstantBuffer* pSkyDomeAlpha = CreateObj<ConstantBuffer>("CBSkyAlpha");
	pSkyDomeAlpha->Create(sizeof(XMFLOAT4));

	// ���f���̐���
	//CreateModel();
	
	// �e�N�X�`���̐���
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pTex[i] = TextureFactory::CreateFromFile(szUIFileName[i]);
	}

	// --- UI�̍쐬 ---
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
	// �^�C�}�[�Z�b�g
	g_pTimer->Set(2.0f, "TitleLogoTimer");		// 2�b��Ƀ^�C�g�����S���o��
	g_pTimer->Set(7.0f, "KeyInputUITimer");		// 7�b��ɃL�[����UI���o��
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

	// �V�[���t�F�[�h�̏I������
	g_pSceneFade->Uninit();

	// �u���[�̏I������
	//g_pBlur->Uninit();

	// BGM�̒�~
	CSound::Stop(BGM_TITLE);
}
void SceneTitle::Update()
{
	//�@���C�g�̍X�V
	XMFLOAT3 color;
	color = g_pLightManager->GetSkyColor();
	g_pLightManager->Update();

	// ��̍X�V
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

	// �Ԃ̐F��ύX����
	ImGui::ColorEdit4(u8"MudSphereColor", g_flowerColor);

	if (IsKeyPress(VK_LEFT))
		pos.y -= 0.1f;
	if (IsKeyPress(VK_RIGHT))
		pos.y += 0.1f;

	// Enter�L�[���Q�[���V�[���ɑJ�ڂ���
	if (IsKeyPress(VK_RETURN))
		g_pSceneFade->SetFade(SceneFade::FADE_OUT, 1.0f);

	// �^�C�}�[�̍X�V
	g_pTimer->Update();

	// �^�C�g�����S�̉��o�̍X�V
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

	// UI�̍X�V
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i]->Update();
	}

	// �V�[���t�F�[�h�X�V
	g_pSceneFade->Update();

	// �u���[�X�V
	//g_pBlur->Update();
}
#pragma region draw
void SceneTitle::Draw()
{
	// �萔�o�b�t�@�̎擾
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pFlowerColorCB = GetObj<ConstantBuffer>("CBFlowerColor");
	ConstantBuffer* pDirectionalLightCB = GetObj<ConstantBuffer>("CBDirectionalLight");
	
	// �����̐ݒ�
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
		// ���_�V�F�[�_�[�̐ݒ�
		SetVertexShader(pWVP, pCamera, m_sceneObj[i].pModel);

		// �Ԃт�̐F��ς���
		if (m_sceneObj[i].name == "flowerpetalModel")
		{
			pFlowerColorCB->Write(g_flowerColor);
			pFlowerColorCB->BindPS(0);
		}
		// �I�u�W�F�N�g�̕`��
		GetObj<Model>(m_sceneObj[i].name)->Draw();
	}

	// ��̕`��
	g_pSkydome->Draw();

	// �A���t�@�u�����h�ݒ�
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);

	// UI�`��
	for (int i = 0; i < TITLEUI_MAX; ++i)
	{
		g_pObjUI[i]->Draw();
	}

	// �V�[���t�F�[�h�`��
	g_pSceneFade->Draw();
	CBlendState::SetZBuffer(true);
}
#pragma endregion 

// �I�u�W�F�N�g���̐ݒ�
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

// ���_�V�F�[�_�[�̐ݒ�
void SetVertexShader(ConstantBuffer* cb, CameraBase* camera, Model* model)
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	DirectX::XMFLOAT4X4 mat[3];

	// --- ���[���h���W���̊i�[ ---
	// ��]
	worldMatrix = XMMatrixRotationX(XMConvertToRadians(model->m_vertex.rot.x));
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(model->m_vertex.rot.y));
	worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(model->m_vertex.rot.z));

	// ���W�ϊ�
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
