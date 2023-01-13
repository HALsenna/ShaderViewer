#include "SceneView.h"
#include "Model.h"
#include "ShaderList.h"
#include "ConstantBuffer.h"
#include "SceneShading.h"
#include "SceneBump.h"
#include "SceneToon.h"
#include "SceneShadow.h"
#include "SceneSelect.h"
#include "SceneDeffered.h"
#include "SceneBaseShader.h"
#include "SceneTitle.h"
#include "Input.h"
#include "CameraDCC.h"
#include "SceneFade.h"

struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camLook;
	DirectX::XMFLOAT3 camUp;
	int index;
};

const char* SettingFileName = "Assets/setting.dat";

void SceneView::Init()
{
	// モデルの読込
	Model* pModel = CreateObj<Model>("UnityModel");		// 読み込んだモデルに名前を指定する
	pModel->Load("Assets/Unitychan/unitychan.fbx", 0.01f);

	// 定数バッファの作成
	ConstantBuffer* pWVP = CreateObj<ConstantBuffer>("CBWVP");
	pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);
	ConstantBuffer* pLight = CreateObj<ConstantBuffer>("CBLight");
	pLight->Create(sizeof(DirectX::XMFLOAT4));
	ConstantBuffer* pDirectionLightCB = CreateObj<ConstantBuffer>("CBDirectionalLight");
	pDirectionLightCB->Create(sizeof(XMFLOAT4) * 2);

	ViewSetting setting =
	{
		DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		SCENE_SELECT
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "rb");
	if (fp)
	{
		fread(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}

	CameraBase* pCamera = CreateObj<CameraDCC>("CameraDCC");
	pCamera->SetPos(setting.camPos);
	pCamera->SetLook(setting.camLook);
	pCamera->SetUp(setting.camUp);
	m_index = setting.index;
	switch (m_index)
	{
	case SCENE_TITLE:		AddSubScene<SceneTitle>(); break;
	case SCENE_SELECT:		AddSubScene<SceneSelect>(); break;
	case SCENE_GAME:		AddSubScene<SceneBaseShader>(); break;
	}
}
void SceneView::Uninit()
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ViewSetting setting =
	{
		pCamera->GetPos(),
		pCamera->GetLook(),
		pCamera->GetUp(),
		m_index
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "wb");
	if (fp)
	{
		//fwrite(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}
}
void SceneView::Update()
{
	int idx = SCENE_NONE;

	if (IsKeyPress(VK_RETURN))
	{
		idx = SCENE_SELECT;
	}

	if (idx != SCENE_NONE && idx != m_index)
	{
		m_index = idx;
		RemoveSubScene();
		switch (m_index)
		{
		case SCENE_TITLE:		AddSubScene<SceneTitle>(); break;
		case SCENE_SELECT:		AddSubScene<SceneSelect>(); break;
		case SCENE_GAME:		AddSubScene<SceneBaseShader>(); break;
		}
	}
	
}
void SceneView::Draw()
{
}

void SceneView::SceneChange(SceneKind scene)
{
	int sceneIdx = scene;

	if(sceneIdx != SCENE_NONE)
		LoadScene(sceneIdx);
}

void SceneView::LoadScene(int index)
{
	m_index = index;
	RemoveSubScene();
	switch (m_index)
	{
	case SCENE_TITLE:		AddSubScene<SceneTitle>(); break;
	case SCENE_GAME:		AddSubScene<SceneBaseShader>(); break;
	}
}
