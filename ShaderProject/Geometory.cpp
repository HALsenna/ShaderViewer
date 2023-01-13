#include "Geometory.h"
#include "MeshBuffer.h"
#include "Shader.h"
#include <DirectXMath.h>

using namespace DirectX;
#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���

//--- �萔
const int GeometoryLineNum = 1000;
const int GeometoryLineVtxNum = GeometoryLineNum * 2;

//--- �\����
struct GeometoryVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

// �O�p�|���S���`��p�\����
struct TriangleVertex
{
	XMFLOAT3 pos;		// ���_���W
};

// �V�F�[�_�[�ɓn���l
struct SHADERBUFFER
{
	XMFLOAT4X4 world;		// ���[���h���W
	XMFLOAT4X4 view;			// �r���[���W
	XMFLOAT4X4 proj;			// �v���W�F�N�V�������W
	XMFLOAT4X4 tex;			// �e�N�X�`�����
};

//--- �O���[�o���ϐ�
UINT g_geometoryLineIndex;
GeometoryVertex g_geometoryLineVtx[GeometoryLineVtxNum];
std::shared_ptr<MeshBuffer> g_pGeometoryLinesMesh;
std::shared_ptr<MeshBuffer> g_pGeometoryBoxMesh;
std::shared_ptr<MeshBuffer> g_pGeometoryPolygonMesh;
std::shared_ptr<VertexShader> g_pGeometoryVS;
std::shared_ptr<PixelShader> g_pGeometoryPS;

XMFLOAT4X4 g_geometoryWVPMat[4];
XMFLOAT4 g_geometoryColor;
XMFLOAT4X4 g_mTex;

// �|���S���̏�����
XMVECTOR Screenpos(XMVECTOR World_Pos);

#pragma region init geometory
void CGeometory::InitGeometory()
{
	// ����
	MeshBuffer::Description lineDesc = {};
	lineDesc.pVtx = g_geometoryLineVtx;
	lineDesc.vtxSize = sizeof(GeometoryVertex);
	lineDesc.vtxCount = GeometoryLineVtxNum;
	lineDesc.isWrite = true;
	lineDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	g_geometoryLineIndex = 0;
	g_pGeometoryLinesMesh = std::make_shared<MeshBuffer>(lineDesc);

	// �{�b�N�X
	const float d = 0.5f;
	DirectX::XMFLOAT4 boxColor(1.0f, 1.0f, 1.0f, 1.0f);
	GeometoryVertex boxVtx[] = {
		{DirectX::XMFLOAT3(-d, d,-d), boxColor},
		{DirectX::XMFLOAT3(d, d,-d), boxColor},
		{DirectX::XMFLOAT3(-d,-d,-d), boxColor},
		{DirectX::XMFLOAT3(d,-d,-d), boxColor},
		{DirectX::XMFLOAT3(-d, d, d), boxColor},
		{DirectX::XMFLOAT3(d, d, d), boxColor},
		{DirectX::XMFLOAT3(-d,-d, d), boxColor},
		{DirectX::XMFLOAT3(d,-d, d), boxColor},
	};
	BYTE boxIdx[] = {
		0,1,2, 2,1,3,
		1,5,3, 3,5,7,
		5,4,7, 7,4,6,
		4,0,6, 6,0,2,
		4,5,0, 0,5,1,
		2,3,6, 6,3,7
	};
	MeshBuffer::Description boxDesc = {};
	boxDesc.pVtx = boxVtx;
	boxDesc.vtxSize = sizeof(GeometoryVertex);
	boxDesc.vtxCount = _countof(boxVtx);
	boxDesc.pIdx = boxIdx;
	boxDesc.idxSize = sizeof(boxIdx[0]);
	boxDesc.idxCount = _countof(boxIdx);
	boxDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	g_pGeometoryBoxMesh = std::make_shared<MeshBuffer>(boxDesc);

	InitPolygonVertex();		// �|���S���o�[�e�b�N�X�̐���

	// ���_�V�F�[�_�[
	g_pGeometoryVS = std::make_shared<VertexShader>();
	g_pGeometoryVS->Compile(R"EOT(
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};

// �p�����[�^
struct VS_INPUT {
	float3	pos	: POSITION;
	float2	uv	: TEXCOORD0;
};

struct VS_OUTPUT {
	float4	pos	: SV_Position;
	float2	uv	: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = mul(float4(input.pos, 1.0f), g_mWorld);
	output.pos = mul(output.pos, g_mView);
	output.pos = mul(output.pos, g_mProjection);
	output.uv = input.uv;
	return output;
}
)EOT");

	// �s�N�Z���V�F�[�_�[
	g_pGeometoryPS = std::make_shared<PixelShader>();
	g_pGeometoryPS->Compile(R"EOT(
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};
float4 g_color : register(b1);		// �F���
cbuffer rate : register(b2)			// ���o�̊���
{
	float rate;
	float3 dummy;
}

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	uv	: TEXCOORD0;
};

Texture2D    g_texture : register(t0);		// �e�N�X�`��
Texture2D	 g_dissolveTex : register(t1);	// �f�B�]���u�p�̃e�N�X�`��
SamplerState samp : register(s0);			// �T���v��

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = g_texture.Sample(samp, input.uv) * g_color;
	float dissolve = g_dissolveTex.Sample(samp, input.uv).r;
	float show = step(rate, dissolve); 
	Color.a *= show;
	return Color;
}
)EOT");

	// �萔�o�b�t�@
	m_pGeometoryWVP = std::make_shared<ConstantBuffer>();
	m_pGeometoryWVP->Create(sizeof(XMFLOAT4X4) * 3);
	m_pGeometoryColor = std::make_shared<ConstantBuffer>();
	m_pGeometoryColor->Create(sizeof(XMFLOAT4));
	m_pDissolveRate = new ConstantBuffer();
	m_pDissolveRate->Create(sizeof(XMFLOAT4));
}
#pragma endregion 

// �|���S���`�撸�_���̍쐬
#pragma region init polygon vertex
void CGeometory::InitPolygonVertex()
{
	m_vertices[0] = {
			XMFLOAT3{ -0.5f, 0.5f, 0.0f},
			XMFLOAT3{ 0.0f, 0.0f, 1.0f },
	};
	m_vertices[1] = {
			XMFLOAT3{ 0.5f, 0.5f, 0.0f },
			XMFLOAT3{ 1.0f, 0.0f, 1.0f }
	};
	m_vertices[2] = {
			XMFLOAT3{ 0.5f, -0.5f, 0.0f },
			XMFLOAT3{ 1.0f, 1.0f, 1.0f}
	};
	m_vertices[3] = {
			XMFLOAT3{ -0.5f, -0.5f, 0.0f },
			XMFLOAT3{ 0.0f, 1.0f, 1.0f}
	};

	BYTE polygonIdx[] = {
		0,1,2, 0,2,3
	};

	// �F�̏�����
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_dissolverate = 0.0f;

	MeshBuffer::Description polygonDesc = {};
	polygonDesc.pVtx = m_vertices;
	polygonDesc.vtxSize = sizeof(PolygonVertex);
	polygonDesc.vtxCount = _countof(m_vertices);
	polygonDesc.pIdx = polygonIdx;
	polygonDesc.idxSize = sizeof(polygonIdx[0]);
	polygonDesc.idxCount = _countof(polygonIdx);
	polygonDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	g_pGeometoryPolygonMesh = std::make_shared<MeshBuffer>(polygonDesc);
}
#pragma endregion 

void CGeometory::UninitGeometory()
{
	g_pGeometoryLinesMesh.reset();
	g_pGeometoryBoxMesh.reset();
	g_pGeometoryVS.reset();
	g_pGeometoryPS.reset();
	m_pGeometoryWVP.reset();
	m_pGeometoryColor.reset();
}

void CGeometory::SetGeometoryVP(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	g_geometoryWVPMat[1] = view;
	g_geometoryWVPMat[2] = proj;
}
void CGeometory::SetGeometoryColor(DirectX::XMFLOAT4 color)
{
	g_geometoryColor = color;
}
void CGeometory::DrawBox(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rot)
{
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[0], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(size.x, size.y, size.z) *
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rot.x)) *
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rot.y)) *
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rot.z)) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
	));
	m_pGeometoryWVP->Write(g_geometoryWVPMat);
	m_pGeometoryWVP->BindVS(0);
	m_pGeometoryWVP->BindPS(0);
	m_pGeometoryColor->Write(&g_geometoryColor);
	m_pGeometoryColor->BindPS(1);
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryBoxMesh->Draw();
}
void CGeometory::AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end)
{
	if (g_geometoryLineIndex < GeometoryLineNum)
	{
		g_geometoryLineVtx[g_geometoryLineIndex].pos = start;
		g_geometoryLineVtx[g_geometoryLineIndex].color = g_geometoryColor;
		++g_geometoryLineIndex;
		g_geometoryLineVtx[g_geometoryLineIndex].pos = end;
		g_geometoryLineVtx[g_geometoryLineIndex].color = g_geometoryColor;
		++g_geometoryLineIndex;
	}
}
void CGeometory::DrawLines()
{
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[0], DirectX::XMMatrixIdentity());
	m_pGeometoryWVP->Write(g_geometoryWVPMat);
	m_pGeometoryWVP->BindVS(0);
	m_pGeometoryColor->Write(&color);
	m_pGeometoryColor->BindVS(1);
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryLinesMesh->Write(g_geometoryLineVtx);
	g_pGeometoryLinesMesh->Draw();
	g_geometoryLineIndex = 0;
}

void CGeometory::DrawPolygon(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT3 rot)
{
	// �z��̏�����
	SHADERBUFFER shaderCB;
	XMStoreFloat4x4(&shaderCB.world, XMMatrixIdentity());
	XMStoreFloat4x4(&shaderCB.view, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&shaderCB.proj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&shaderCB.tex, XMMatrixIdentity());
	//cb.tex._44 = 0.0f;

	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(size.x, size.y, 1.0f);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rot.x),
		XMConvertToRadians(rot.y), XMConvertToRadians(rot.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(pos.x, pos.y, pos.z);
	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&shaderCB.world, mWorld);

	// �e�N�X�`���ݒ�
	if (m_pTexture)
	{
		// �g�k
		//mWorld = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		// �ړ�
		//mWorld *= XMMatrixTranslation(1.0f, 1.0f, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&shaderCB.tex, mWorld);
	}
	else
	{
		shaderCB.tex._44 = 0.0f;
	}
	if (m_pTexture)
	{
		GetContext()->PSSetShaderResources(0, 1, &m_pTexture);		// UI�e�N�X�`���̐ݒ�
	}
	if (m_pDissolveTex)
	{
		GetContext()->PSSetShaderResources(0, 2, &m_pTexture);	// �f�B�]���u�e�N�X�`���̐ݒ�
	}

	// ���W�̐ݒ�
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[0], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.world)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[1], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.view)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[2], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.proj)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[3], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.tex)));
	m_pGeometoryWVP->Write(g_geometoryWVPMat);
	m_pGeometoryWVP->BindVS(0);
	m_pGeometoryWVP->BindPS(0);

	// �F�̐ݒ�
	XMFLOAT4 cb = XMFLOAT4{ m_color.r, m_color.g, m_color.b, m_color.a };
	m_pGeometoryColor->Write(&cb);
	m_pGeometoryColor->BindPS(1);

	// �f�B�]���u���o�̐ݒ�
	cb = XMFLOAT4{ m_dissolverate, 0.0f, 0.0f, 0.0f };
	m_pDissolveRate->Write(&cb);
	m_pDissolveRate->BindPS(2);

	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryPolygonMesh->Draw();
}

XMVECTOR Screenpos(XMVECTOR World_Pos) 
{
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX g_View = XMMatrixLookAtLH(Eye, At, Up);

	XMMATRIX g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 0.1f, 1000.0f);

	float w = SCREEN_WIDTH / 2.0f;
	float h = SCREEN_HEIGHT / 2.0f;
	XMMATRIX viewport = {
	w, 0, 0, 0,
	0, -h, 0, 0,
	0, 0, 1, 0,
	w, h, 0, 1
	};

	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);
	// z�Ŋ�����-1~1�͈̔͂Ɏ��߂�
	// �X�N���[���ϊ�
	const XMVECTOR view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, 1.0f, 1.0f);
	return XMVector3Transform(view_vec, viewport);
}

// �e�N�X�`���̐ݒ�
void CGeometory::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	m_pTexture = pTexture;
	//g_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

// �A���t�@�l�̐ݒ�
void CGeometory::SetAlpha(float alpha)
{
	m_color.a = alpha;
}

// �F�̐ݒ�
void CGeometory::SetColor(XMFLOAT4 color)
{
	m_color = { color.x, color.y, color.z, color.w };
}

// �f�B�]���u�ݒ�
void CGeometory::SetDissolve(float rate, ID3D11ShaderResourceView* pTexture)
{
	m_dissolverate = rate;
	m_pDissolveTex = pTexture;
}
