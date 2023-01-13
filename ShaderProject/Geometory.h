#ifndef __GEOMETORY_H__
#define __GEOMETORY_H__

#include <Windows.h>
#include <DirectXMath.h>
#include "DirectX.h"
#include <memory>
#include "ConstantBuffer.h"

using namespace DirectX;

// �|���S�����_���\����
struct PolygonVertex
{
	XMFLOAT3 pos;		// ���_���W
	XMFLOAT3 uv;		// UV���W
};

// �F���\����
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
	void SetTexture(ID3D11ShaderResourceView* pTexture);		// �e�N�X�`���̐ݒ�
	void InitPolygonVertex();
	void SetAlpha(float alpha);
	void SetColor(XMFLOAT4 color);
	void SetDissolve(float rate, ID3D11ShaderResourceView* pTexture);

private:
	ID3D11ShaderResourceView* m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	ID3D11ShaderResourceView* m_pDissolveTex = nullptr;	// �e�N�X�`���ւ̃|�C���^
	PolygonVertex m_vertices[4];		//���_�f�[�^�B
	PolygonColor m_color;	// �F���
	float m_dissolverate;
	std::shared_ptr<ConstantBuffer> m_pGeometoryColor;
	std::shared_ptr<ConstantBuffer> m_pGeometoryWVP;
	ConstantBuffer* m_pDissolveRate;
};

#endif // __GEOMETORY_H__