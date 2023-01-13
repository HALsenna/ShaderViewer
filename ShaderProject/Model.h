#ifndef __MODEL_H__
#define __MODEL_H__

#include <DirectXMath.h>
#include <vector>
#include "Shader.h"
#include "MeshBuffer.h"

using namespace DirectX;

class Model
{
public:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		XMFLOAT3 size;
		XMFLOAT3 rot = {0.0f, 0.0f, 0.0f};
	};
	struct Material
	{
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 specular;
		ID3D11ShaderResourceView* pTexture;
	};
	struct Mesh
	{
		Vertex* pVertices;
		unsigned int vertexNum;
		unsigned int* pIndices;
		unsigned int indexNum;
		unsigned int materialID;
		std::vector<uint32_t> boneID;
		MeshBuffer* pMesh;
	};
public:
	Model();
	~Model();
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 GetPos();
	void SetRot(XMFLOAT3 rot);
	XMFLOAT3 GetRot();
	void SetSize(XMFLOAT3 size);
	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);
	const Mesh* GetMesh(unsigned int index);
	unsigned int GetMeshNum();

public:
	bool Load(const char* file, float scale = 1.0f, bool flip = false);
	void Update();
	void Draw();
	bool CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX, XMFLOAT3* pN);


private:
	static VertexShader* m_pDefVS;
	static PixelShader* m_pDefPS;
	static unsigned int m_shaderRef;
private:
	Mesh* m_pMeshes;
	unsigned int m_meshNum;
	Material* m_pMaterials;
	unsigned int m_materialNum;
	VertexShader* m_pVS;
	PixelShader* m_pPS;

public:
	XMMATRIX m_mWorld;		// ÉèÅ[ÉãÉhïœä∑
	float m_smooth = 1.0f;
	Vertex m_vertex;
};


#endif // __MODEL_H__