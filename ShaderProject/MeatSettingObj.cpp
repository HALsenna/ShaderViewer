#include "MeatSettingObj.h"

// �萔��`
namespace
{
	const float SPEED_ROT = 1.0f;
}

// ���f���̃t�@�C����
const char* szObjFileName = "Assets/Model/model_meat.fbx";


CMeat_Setting::CMeat_Setting(SCENEOBJECTINFO info)
{
	// �I�u�W�F�N�g�����i�[����
	m_ObjectInfo = info;
	Init();
}

CMeat_Setting::~CMeat_Setting()
{

}

void CMeat_Setting::Init()
{
	Model* pModel = m_ObjectInfo.pModel;
	// ���f���ɏ���ݒ肷��
	pModel->Load(szObjFileName, m_ObjectInfo.size.x);
	pModel->SetPos(m_ObjectInfo.pos);		// ���W
	pModel->SetSize(m_ObjectInfo.size);		// �g�嗦
	pModel->SetRot(m_ObjectInfo.rot);		// ��]
	pModel->SetVertexShader(GetVS(m_ObjectInfo.vs));	// ���_�V�F�[�_�[
	pModel->SetPixelShader(GetPS(m_ObjectInfo.ps));		// �s�N�Z���V�F�[�_�[
}

void CMeat_Setting::Update()
{
	Model* pModel = m_ObjectInfo.pModel;

	// ��]
	XMFLOAT3 rot = pModel->GetRot();
	rot.y += SPEED_ROT;
	pModel->SetRot(rot);
}

void CMeat_Setting::Draw()
{

}
