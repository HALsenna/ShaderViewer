#include "SkyDome.h"
#include "ShaderList.h"

// ƒ‚ƒfƒ‹‚Ìƒtƒ@ƒCƒ‹–¼
static const char* fileName[SKY_MAX] = {
	"Assets/Model/SkyDome/NoonSky.fbx",		// ’‹
	//"Assets/Model/teapotSmooth.fbx",
	"Assets/Model/SkyDome/EveningSky.fbx",	// —[•û
	"Assets/Model/SkyDome/NightSky.fbx",	// –é
};

CSkyDome::CSkyDome(Model* pModel[SKY_MAX])
{
	for (int i = 0; i < SKY_MAX; ++i)
	{
		m_pModel[i] = pModel[i];
		m_pModel[i]->Load(fileName[i], 1.0f);
		m_pModel[i]->SetVertexShader(GetVS(VS_OBJECT));
		m_pModel[i]->SetPixelShader(GetPS(PS_SKYDOME));
		pModel[i]->m_vertex.pos = { 0.0f, 0.0f, 0.0f };
		pModel[i]->m_vertex.rot = { 0.0f, -90.0f, 0.0f };
	}
	Init();
}

CSkyDome::~CSkyDome()
{
}

void CSkyDome::Init()
{
	m_fTime = 0.0f;
	m_skyKind = SKY_NOON;
}

void CSkyDome::Update()
{
	m_fTime += 1.0f / 60.0f;

	if (m_fTime > TIME_SKYCHANGE)
	{
		m_fTime = 0.0f;
		int nCurrentKind = (int)m_skyKind;
		nCurrentKind++;
		m_skyKind = (SKYKIND)nCurrentKind;
		if (m_skyKind == SKY_MAX)
			m_skyKind = (SKYKIND)0;
	}
}

void CSkyDome::Draw()
{
	XMMATRIX worldMatrix = XMMatrixIdentity();
	DirectX::XMFLOAT4X4 mat[3];

	// --- ƒ[ƒ‹ƒhÀ•Wî•ñ‚ÌŠi”[ ---
	// ‰ñ“]
	worldMatrix = XMMatrixRotationX(XMConvertToRadians(m_pModel[m_skyKind]->m_vertex.rot.x));
	worldMatrix *= XMMatrixRotationY(XMConvertToRadians(m_pModel[m_skyKind]->m_vertex.rot.y));
	worldMatrix *= XMMatrixRotationZ(XMConvertToRadians(m_pModel[m_skyKind]->m_vertex.rot.z));

	// À•W•ÏŠ·
	worldMatrix *= XMMatrixTranslation(m_pModel[m_skyKind]->m_vertex.pos.x, m_pModel[m_skyKind]->m_vertex.pos.y, m_pModel[m_skyKind]->m_vertex.pos.z);
	//XMFLOAT4X4 world = XMMatrixMultiply(worldMatrix, test);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(worldMatrix));;
	// --------------------------
	mat[1] = m_camera->GetView();
	mat[2] = m_camera->GetProj();
	m_cb->Write(mat);
	m_cb->BindVS(0);

	//m_alpha->Write();
	m_pModel[m_skyKind]->Draw();
}

void CSkyDome::SetCamera(CameraBase* camera,  ConstantBuffer* cb)
{
	m_camera = camera;
	//m_alpha = AlphaCB;
	m_cb = cb;
}

void CSkyDome::SetSkyKind(SKYKIND kind)
{
	m_skyKind = kind;
}

SKYKIND CSkyDome::GetSkyKind()
{
	return m_skyKind;
}
