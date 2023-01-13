#include "Shop.h"
#include "SceneTitle.h"

namespace
{
	const char* tableModel = "Assets/Model/model_table.fbx";
}

CShop::CShop(Model* tableModel)
{
	// テーブルモデルの設定
	m_tableModel = tableModel;
}

CShop::~CShop()
{
}

void CShop::Init()
{
}

void CShop::Update()
{
}
