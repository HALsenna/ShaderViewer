#include "SceneSelect.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "CBlendState.h"
#include "CameraBase.h"
#include "Input.h"

// �萔��`
namespace
{
	const float BUTTON_SIZE = 0.4f;
	const float KEYUI_SIZE = 0.3f;
	const float KEYUI_POSY = -300.0f;
	const float KEYUI_CENTER_POSX = -200.0f;
	const float KEYUI_INTERVAL = 50.0f;
}

// ---- UI�̏�� ----
#pragma region UIInitInfo
// �t�@�C����
static const char* szUIFileName[SELECT_MAX] = {
	"Assets/Texture/MenuUI/TEX_BG_Plane.png",		// �w�i
	"Assets/Texture/MenuUI/TEX_Menu_Meat.png",		// �I������(����)
	"Assets/Texture/MenuUI/TEX_Menu_UI.png",		// �I������(UI)
	"Assets/Texture/MenuUI/TEX_Setting_BG.png",		// �ݒ荀��(�w�i)
	"Assets/Texture/MenuUI/TEX_Button_Decide.png",	// ����{�^��
	"Assets/Texture/MenuUI/TEX_Key_1.png",			// �L�[Num1
	"Assets/Texture/MenuUI/TEX_Key_2.png",			// �L�[Num2
	"Assets/Texture/MenuUI/TEX_Key_3.png",			// �L�[Num3
	"Assets/Texture/MenuUI/TEX_ChangeLight.png",	// ���C�g�ؑ�UI
};

// UI���
const UIINFO g_UIInfo[SELECT_MAX] = {
	{{0.0f, 0.0f, 0.0f},{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// �w�i
	{{-500.0f, 250.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// �I������(����)
	{{-500.0f, 180.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// �I������(UI)
	{{400.0f, 50.0f, 0.0f},{0.4f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},	// �I������(�w�i)
	{{400.0f, -100.0f, 0.0f},{0.4f, 0.4f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},	// ����{�^��
	{{KEYUI_CENTER_POSX - KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// �L�[Num1
	{{KEYUI_CENTER_POSX, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},						// �L�[Num2
	{{KEYUI_CENTER_POSX + KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// �L�[Num3
	{{50.0f, KEYUI_POSY, 0.0f},{0.7f, 0.7f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr},		// ���C�g�ؑ�UI
};
#pragma endregion
// ------------------

// ---- ���f���̏������ ----
#pragma region ModelInitInfo
// �I�u�W�F�N�g��
const char* g_szObjectName[SELECTOBJ_MAX] = {
	"Meat",		// ����
};

// ���W
const XMFLOAT3 g_ObjectPos[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// ����
};

// ��]
const XMFLOAT3 g_ObjectRot[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// ����
};

// �T�C�Y
const XMFLOAT3 g_ObjectSize[SELECTOBJ_MAX] = {
	{1.0f, 1.0f, 1.0f},					// ����
};

// ���_�V�F�[�_�[
const VertexShaderKind g_vShader[SELECTOBJ_MAX] = {
	VS_OBJECT,				// ����
};

// �s�N�Z���V�F�[�_�[
const PixelShaderKind g_pShader[SELECTOBJ_MAX] = {
	PS_OBJECT,				// ����
};
#pragma endregion 
// -----------------------

// ---- ���̓L�[ ----
#pragma region inputkey
namespace
{
	const BYTE KEY_CHANGELIGHT_NORMAL = '1';	// �ʏ���
	const BYTE KEY_CHANGELIGHT_INSIDE =	'2';	// ������
	const BYTE KEY_CHANGELIGHT_NIGHT = '3';		// ��	
}
#pragma endregion 
// ----------------

// �O���[�o���ϐ�
static CMeat_Setting* g_pMeatObj;		// �����I�u�W�F�N�g

// �v���g�^�C�v�錾
UIINFO SetUpUI(UIINFO);
CUI* CreateUI(UIINFO);

void SceneSelect::Init()
{
	// UI�̏�����
	InitUI();
	// 3D�̏�����
	Init3D();
	// �����̏�����
	InitLight();
	
}
void SceneSelect::Uninit()
{
	m_pObjData.clear();
	m_pObjData.clear();
}
void SceneSelect::Update()
{
	// UI�̍X�V
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		m_pUIObj[i]->Update();
	}

	// �L�[���͎�t�X�V
	UpdateKeyInput();
	// �����X�V
	g_pMeatObj->Update();
}
void SceneSelect::Draw()
{
	// �A���t�@�u�����h�ݒ�
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);
	
	// �w�i�`��
	m_pUIObj[SELECT_BG_PLANE]->Draw();
	
	// �A���t�@�u�����h�ݒ�����ɖ߂�
	CBlendState::SetZBuffer(true);
	
	// 3D�I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_pObjData.size(); ++i)
	{
		SetShader((SELECTSCENE_OBJECT) i);			// �V�F�[�_�[�̐ݒ�
		m_pObjData[i]->Draw();
	}

	// �A���t�@�u�����h�ݒ�
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);

	// UI�̕`��
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// �w�i�ȊO��`�悷��
		if (i != (int)SELECT_BG_PLANE)
			m_pUIObj[i]->Draw();
	}
	
	// �A���t�@�u�����h�ݒ�����ɖ߂�
	CBlendState::SetZBuffer(true);
}

/*
	@brief UI�̏�����
	@details UI�̍��W�A�F�A�e�N�X�`����ݒ肵�A�I�u�W�F�N�g�𐶐�����
*/
void SceneSelect::InitUI()
{
	Texture* pTex;
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// UI�̏����ݒ�
		m_aUIInitData[i] = SetUpUI(g_UIInfo[i]);
		pTex = TextureFactory::CreateFromFile(szUIFileName[i]);
		m_aUIInitData[i].pTex = pTex;
		// UI�̐���
		m_pUIObj[i] = CreateUI(m_aUIInitData[i]);
	}
}

/*
	@brief ���f���̏�����
	@details ���f���̍��W�𐶐�����
*/
void SceneSelect::InitModel()
{
	SCENEOBJECTINFO obj;		// ���i�[�p
	
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		obj.name = g_szObjectName[i];	// �I�u�W�F�N�g���O
		obj.pos = g_ObjectPos[i];		// ���W
		obj.rot = g_ObjectRot[i];		// ��]
		obj.size = g_ObjectSize[i];		// �T�C�Y
		obj.vs = g_vShader[i];			// ���_�V�F�[�_�[
		obj.ps = g_pShader[i];			// �s�N�Z���V�F�[�_�[
		
		// ���̃Z�b�g
		m_ObjInitData.push_back(obj);
	}
}

// ���f���̐���
void SceneSelect::CreateModel()
{
	Model* pModel;		// ���i�[�p
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		pModel = CreateObj<Model>(g_szObjectName[i]);
		m_ObjInitData[i].pModel = pModel;
		m_pObjData.push_back(pModel);
	}
}

// �����̏�����
void SceneSelect::InitLight()
{
	m_pSceneLight = new CSelectSceneLight();
}

// UI�̏����ݒ�
UIINFO SetUpUI(UIINFO info)
{
	UIINFO pData;
	pData.pos = info.pos;			// ���W
	pData.size = info.size;			// �T�C�Y
	pData.color = info.color;		// �F
	//pData.pTex = info.pTex;			// �e�N�X�`��
	return pData;
}

// UI�̐���
CUI* CreateUI(UIINFO data)
{
	CUI* pUI;
	pUI = new CUI(data.pos, data.size, data.color, data.pTex);
	return pUI;
}

void SceneSelect::UpdateKeyInput()
{
	// ���C�g�ύX(�ʏ���)
	if (IsKeyPress(KEY_CHANGELIGHT_NORMAL))
	{
		m_pSceneLight->ChangeLight(LIGHT_NORMAL);
	}
	// ���C�g�ύX(������)
	else if (IsKeyPress(KEY_CHANGELIGHT_INSIDE))
	{
		m_pSceneLight->ChangeLight(LIGHT_INSIDE);
	}
	// ���C�g�̕ύX(��)
	else if (IsKeyPress(KEY_CHANGELIGHT_NIGHT))
	{
		m_pSceneLight->ChangeLight(LIGHT_NIGHT);
	}
}

// 3D�̏�����
void SceneSelect::Init3D()
{
	// ���f���̏�����
	InitModel();
	// ���f���̐���
	CreateModel();
	// �I�u�W�F�N�g�̐���
	CreateObject();
}

// �I�u�W�F�N�g�̐���
void SceneSelect::CreateObject()
{
	// �����I�u�W�F�N�g
	g_pMeatObj = new CMeat_Setting(m_ObjInitData[SELECTOBJ_MEAT]);
}

// �V�F�[�_�[�̐ݒ�
void SceneSelect::SetShader(SELECTSCENE_OBJECT kind)
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pDirectionalLightCB = GetObj<ConstantBuffer>("CBDirectionalLight");

	// ---- ���_�V�F�[�_�[�̐ݒ� ----
	XMMATRIX mWorld;		// ���[���h���W
	XMFLOAT4X4 mat[3];

	// ���[���h�ϊ�
	// ��]
	mWorld = XMMatrixRotationX(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.x));
	mWorld *= XMMatrixRotationY(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.y));
	mWorld *= XMMatrixRotationZ(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.z));

	// ���W�ϊ�
	mWorld *= XMMatrixTranslation(m_pObjData[kind]->m_vertex.pos.x, m_pObjData[kind]->m_vertex.pos.y, m_pObjData[kind]->m_vertex.pos.z);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(mWorld));

	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);
	// --------------------------

	// ---- �s�N�Z���V�F�[�_�[�̐ݒ� ----
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
