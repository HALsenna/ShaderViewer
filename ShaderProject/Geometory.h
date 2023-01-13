#ifndef __GEOMETORY_H__
#define __GEOMETORY_H__

#include <Windows.h>
#include <DirectXMath.h>
#include "DirectX.h"
#include <memory>
#include "ConstantBuffer.h"

using namespace DirectX;

// ポリゴン頂点情報構造体
struct PolygonVertex
{
	XMFLOAT3 pos;		// 頂点座標
	XMFLOAT3 uv;		// UV座標
};

// 色情報構造体
struct PolygonColor
{
	float r, g, b, a;	
};

class CGeometory
{
public:
	void InitGeometory();
	void DrawPolygon(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT3 rot);
	void UninitGeometory();
	void SetGeometoryVP(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	void SetGeometoryColor(DirectX::XMFLOAT4 color);
	void DrawBox(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end);
	void DrawLines();
	void SetTexture(ID3D11ShaderResourceView* pTexture);		// テクスチャの設定
	void InitPolygonVertex();
	void SetAlpha(float alpha);
	void SetColor(XMFLOAT4 color);
	void SetDissolve(float rate, ID3D11ShaderResourceView* pTexture);

private:
	ID3D11ShaderResourceView* m_pTexture;		// テクスチャへのポインタ
	ID3D11ShaderResourceView* m_pDissolveTex = nullptr;	// テクスチャへのポインタ
	PolygonVertex m_vertices[4];		//頂点データ。
	PolygonColor m_color;	// 色情報
	float m_dissolverate;
	std::shared_ptr<ConstantBuffer> m_pGeometoryColor;
	std::shared_ptr<ConstantBuffer> m_pGeometoryWVP;
	ConstantBuffer* m_pDissolveRate;
};

#endif // __GEOMETORY_H__