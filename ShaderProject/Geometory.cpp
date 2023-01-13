#include "Geometory.h"
#include "MeshBuffer.h"
#include "Shader.h"
#include <DirectXMath.h>

using namespace DirectX;
#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ

//--- 定数
const int GeometoryLineNum = 1000;
const int GeometoryLineVtxNum = GeometoryLineNum * 2;

//--- 構造体
struct GeometoryVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

// 三角ポリゴン描画用構造体
struct TriangleVertex
{
	XMFLOAT3 pos;		// 頂点座標
};

// シェーダーに渡す値
struct SHADERBUFFER
{
	XMFLOAT4X4 world;		// ワールド座標
	XMFLOAT4X4 view;			// ビュー座標
	XMFLOAT4X4 proj;			// プロジェクション座標
	XMFLOAT4X4 tex;			// テクスチャ情報
};

//--- グローバル変数
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

// ポリゴンの初期化
XMVECTOR Screenpos(XMVECTOR World_Pos);

#pragma region init geometory
void CGeometory::InitGeometory()
{
	// 直線
	MeshBuffer::Description lineDesc = {};
	lineDesc.pVtx = g_geometoryLineVtx;
	lineDesc.vtxSize = sizeof(GeometoryVertex);
	lineDesc.vtxCount = GeometoryLineVtxNum;
	lineDesc.isWrite = true;
	lineDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	g_geometoryLineIndex = 0;
	g_pGeometoryLinesMesh = std::make_shared<MeshBuffer>(lineDesc);

	// ボックス
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

	InitPolygonVertex();		// ポリゴンバーテックスの生成

	// 頂点シェーダー
	g_pGeometoryVS = std::make_shared<VertexShader>();
	g_pGeometoryVS->Compile(R"EOT(
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};

// パラメータ
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

	// ピクセルシェーダー
	g_pGeometoryPS = std::make_shared<PixelShader>();
	g_pGeometoryPS->Compile(R"EOT(
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};
float4 g_color : register(b1);		// 色情報
cbuffer rate : register(b2)			// 演出の割合
{
	float rate;
	float3 dummy;
}

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	uv	: TEXCOORD0;
};

Texture2D    g_texture : register(t0);		// テクスチャ
Texture2D	 g_dissolveTex : register(t1);	// ディゾルブ用のテクスチャ
SamplerState samp : register(s0);			// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = g_texture.Sample(samp, input.uv) * g_color;
	float dissolve = g_dissolveTex.Sample(samp, input.uv).r;
	float show = step(rate, dissolve); 
	Color.a *= show;
	return Color;
}
)EOT");

	// 定数バッファ
	m_pGeometoryWVP = std::make_shared<ConstantBuffer>();
	m_pGeometoryWVP->Create(sizeof(XMFLOAT4X4) * 3);
	m_pGeometoryColor = std::make_shared<ConstantBuffer>();
	m_pGeometoryColor->Create(sizeof(XMFLOAT4));
	m_pDissolveRate = new ConstantBuffer();
	m_pDissolveRate->Create(sizeof(XMFLOAT4));
}
#pragma endregion 

// ポリゴン描画頂点情報の作成
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

	// 色の初期化
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
	// 配列の初期化
	SHADERBUFFER shaderCB;
	XMStoreFloat4x4(&shaderCB.world, XMMatrixIdentity());
	XMStoreFloat4x4(&shaderCB.view, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&shaderCB.proj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&shaderCB.tex, XMMatrixIdentity());
	//cb.tex._44 = 0.0f;

	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(size.x, size.y, 1.0f);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(rot.x),
		XMConvertToRadians(rot.y), XMConvertToRadians(rot.z));
	// 移動
	mWorld *= XMMatrixTranslation(pos.x, pos.y, pos.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&shaderCB.world, mWorld);

	// テクスチャ設定
	if (m_pTexture)
	{
		// 拡縮
		//mWorld = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		// 移動
		//mWorld *= XMMatrixTranslation(1.0f, 1.0f, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&shaderCB.tex, mWorld);
	}
	else
	{
		shaderCB.tex._44 = 0.0f;
	}
	if (m_pTexture)
	{
		GetContext()->PSSetShaderResources(0, 1, &m_pTexture);		// UIテクスチャの設定
	}
	if (m_pDissolveTex)
	{
		GetContext()->PSSetShaderResources(0, 2, &m_pTexture);	// ディゾルブテクスチャの設定
	}

	// 座標の設定
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[0], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.world)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[1], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.view)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[2], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.proj)));
	DirectX::XMStoreFloat4x4(&g_geometoryWVPMat[3], XMMatrixTranspose(XMLoadFloat4x4(&shaderCB.tex)));
	m_pGeometoryWVP->Write(g_geometoryWVPMat);
	m_pGeometoryWVP->BindVS(0);
	m_pGeometoryWVP->BindPS(0);

	// 色の設定
	XMFLOAT4 cb = XMFLOAT4{ m_color.r, m_color.g, m_color.b, m_color.a };
	m_pGeometoryColor->Write(&cb);
	m_pGeometoryColor->BindPS(1);

	// ディゾルブ演出の設定
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

	// ビュー変換とプロジェクション変換
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);
	// zで割って-1~1の範囲に収める
	// スクリーン変換
	const XMVECTOR view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, 1.0f, 1.0f);
	return XMVector3Transform(view_vec, viewport);
}

// テクスチャの設定
void CGeometory::SetTexture(ID3D11ShaderResourceView* pTexture)
{
	m_pTexture = pTexture;
	//g_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
}

// アルファ値の設定
void CGeometory::SetAlpha(float alpha)
{
	m_color.a = alpha;
}

// 色の設定
void CGeometory::SetColor(XMFLOAT4 color)
{
	m_color = { color.x, color.y, color.z, color.w };
}

// ディゾルブ設定
void CGeometory::SetDissolve(float rate, ID3D11ShaderResourceView* pTexture)
{
	m_dissolverate = rate;
	m_pDissolveTex = pTexture;
}
