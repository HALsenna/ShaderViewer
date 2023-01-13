#include "MeatSettingObj.h"

// 定数定義
namespace
{
	const float SPEED_ROT = 1.0f;
}

// モデルのファイル名
const char* szObjFileName = "Assets/Model/model_meat.fbx";


CMeat_Setting::CMeat_Setting(SCENEOBJECTINFO info)
{
	// オブジェクト情報を格納する
	m_ObjectInfo = info;
	Init();
}

CMeat_Setting::~CMeat_Setting()
{

}

void CMeat_Setting::Init()
{
	Model* pModel = m_ObjectInfo.pModel;
	// モデルに情報を設定する
	pModel->Load(szObjFileName, m_ObjectInfo.size.x);
	pModel->SetPos(m_ObjectInfo.pos);		// 座標
	pModel->SetSize(m_ObjectInfo.size);		// 拡大率
	pModel->SetRot(m_ObjectInfo.rot);		// 回転
	pModel->SetVertexShader(GetVS(m_ObjectInfo.vs));	// 頂点シェーダー
	pModel->SetPixelShader(GetPS(m_ObjectInfo.ps));		// ピクセルシェーダー
}

void CMeat_Setting::Update()
{
	Model* pModel = m_ObjectInfo.pModel;

	// 回転
	XMFLOAT3 rot = pModel->GetRot();
	rot.y += SPEED_ROT;
	pModel->SetRot(rot);
}

void CMeat_Setting::Draw()
{

}
