#include "Player.h"
#include "ShaderList.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "CameraDCC.h"
#include <stdlib.h>
#include "Sound.h"

// モデルのファイル名
static const char* fileName = "Assets/Model/model_drone.fbx";

CPlayer::CPlayer(Model* pModel, XMFLOAT3 pos)
{
	m_pModel = pModel;
	m_pModel->Load(fileName, 0.001f);
	m_pModel->SetVertexShader(GetVS(VS_OBJECT));
	m_pModel->SetPixelShader(GetPS(PS_PLAYER));
	pModel->m_vertex.pos = pos;
	Init();
}

CPlayer::~CPlayer()
{

}

void CPlayer::Init()
{
	canMove = false;
	m_healPointRange = 1.5f;
}

void CPlayer::Update(XMFLOAT3 healPos)
{
	/*if (IsKeyPress(VK_UP))
	{
		m_pModel->m_vertex.pos.z += 0.01f;
	}
	else if (IsKeyPress(VK_DOWN))
	{
		m_pModel->m_vertex.pos.z -= 0.01f;
	}
	else if (IsKeyPress(VK_RIGHT))
	{
		m_pModel->m_vertex.pos.x += 0.01f;
	}
	else if (IsKeyPress(VK_LEFT))
	{
		m_pModel->m_vertex.pos.x -= 0.01f;
	}*/

	// 回復ポイントにいる場合
	//XMFLOAT3 distance;
	
	if (abs(m_pModel->m_vertex.pos.x - healPos.x) < m_healPointRange
		&& abs(m_pModel->m_vertex.pos.y - healPos.y) < m_healPointRange
		&& abs(m_pModel->m_vertex.pos.z - healPos.z) < m_healPointRange)
	{
		if (!m_isHealPoint)
		{
			m_isHealPoint = true;
			CSound::Play(SE_HEAL);
		}
	}
	else
	{
		m_isHealPoint = false;
	}

	m_pModel->m_vertex.pos.x = m_camera->GetPos().x;
	m_camera->SetPosY(m_pModel->m_vertex.pos.y + 0.5f);
	m_pModel->m_vertex.pos.z = m_camera->GetPos().z + 1.0f;
	//m_pModel->m_vertex.rot.y += XMConvertToRadians(m_camera->m_camFront);
	ImGui::Text("playerPos: %.3f, %.3f, %.3f", m_pModel->m_vertex.pos.x, m_pModel->m_vertex.pos.y, m_pModel->m_vertex.pos.z);
	ImGui::Text("Front:%.3f", m_camera->m_camFront);
	//m_camera->SetPos(m_pModel->m_vertex.pos);
}