#include "SceneBump.h"
#include "Model.h"
#include "TextureFactory.h"
#include "ShaderList.h"
#include "ConstantBuffer.h"
#include "CameraBase.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

// �v���g�^�C�v�錾
MeshBuffer** CreateBumpModel(Model* _pModel);

struct BumpVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 tangent;		// �ڃx�N�g��(�^���W�F���g�x�N�g��)
	// ���[���h��ԏ�Ńe�N�X�`����\��t���鉡�̕���
};

void SceneBump::Init()
{
	// �n�߂͓D�̋��̂�`�悷��
	m_viewObj = OBJECT_MUDSPHERE;

	// Unity����񃂃f���̐ݒ�
	#pragma region set unity model
	Model* pModel = GetObj<Model>("UnityModel");
	pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_DISSOLVE));

	/*MeshBuffer** ppMesh = CreateBumpModel(pModel);
	RegisterObj<MeshBuffer*>("UnityBump", ppMesh);*/
	#pragma endregion 

	//�n�`�̓Ǎ�
	#pragma region set ground model
	pModel = CreateObj<Model>("GroundModel");
	pModel->Load("Assets/Model/Ground.fbx");
	pModel->SetVertexShader(GetVS(VS_SEA));
	pModel->SetPixelShader(GetPS(PS_SEA));
	MeshBuffer** ppMesh = CreateBumpModel(pModel);
	RegisterObj<MeshBuffer*>("GroundBump", ppMesh);
	#pragma endregion 

	// ���̓Ǎ�
	#pragma region set mud sphere model
	pModel = CreateObj<Model>("MudSphereModel");
	pModel->Load("Assets/Model/Sphere.fbx");
	pModel->SetVertexShader(GetVS(VS_MUD));
	pModel->SetPixelShader(GetPS(PS_MUD));
	ppMesh = CreateBumpModel(pModel);
	RegisterObj<MeshBuffer*>("MudSphereBump", ppMesh);
	#pragma endregion 

	// �R���N���[�g�u���b�N�̓ǂݍ���
	#pragma region set concrete model
	pModel = CreateObj<Model>("ConcreteModel");
	pModel->Load("Assets/Model/Cube.fbx");
	pModel->SetVertexShader(GetVS(VS_CONCRETE));
	pModel->SetPixelShader(GetPS(PS_CONCRETE));
	ppMesh = CreateBumpModel(pModel);
	RegisterObj<MeshBuffer*>("ConcreteBump", ppMesh);
	#pragma endregion 

	//�@���}�b�v�̓Ǎ�
	#pragma region set normalmap
	// ��̖@���}�b�v
	Texture* pTex = TextureFactory::CreateFromFile("Assets/block_normal.png");
	RegisterObj<Texture>("NormalMap", pTex);

	// �D�̖@���}�b�v
	pTex = TextureFactory::CreateFromFile("Assets/mud_normal.png");
	RegisterObj<Texture>("MudTex", pTex);

	// �C�̖@���}�b�v
	pTex = TextureFactory::CreateFromFile("Assets/seawave_normal.png");
	RegisterObj<Texture>("SeaWaveTex", pTex);

	// �R���N���[�g�̖@���}�b�v
	pTex = TextureFactory::CreateFromFile("Assets/concrete_normal.png");
	RegisterObj<Texture>("ConcreteTex", pTex);
	#pragma endregion 

	// �o�b�t�@�̓o�^
	#pragma region set constant buffer
	// �J����
	ConstantBuffer* pCamBuf = CreateObj<ConstantBuffer>("CameraCB");
	pCamBuf->Create(sizeof(DirectX::XMFLOAT4) * 2);
	// ���V�F�[�_�[�̃J���[���
	ConstantBuffer* pColor = CreateObj<ConstantBuffer>("Color");
	pColor->Create(sizeof(DirectX::XMFLOAT4));
	// ���W�F�[�_�[�̃e�N�X�`�����W
	ConstantBuffer* pTexPos = CreateObj<ConstantBuffer>("TexPos");
	pTexPos->Create(sizeof(DirectX::XMFLOAT4));
	#pragma endregion 

	m_rad = 0.0f;
	m_seaLightRad = 0.0f;
}
void SceneBump::Uninit()
{
}
void SceneBump::Update()
{
	Model* pModel = GetObj<Model>("UnityModel");
	if (IsKeyPress(VK_LEFT))
	{
		m_rad -= 0.01f;
	}
	if (IsKeyPress(VK_RIGHT))
	{
		m_rad += 0.01f;
	}
	m_seaLightRad += 0.01f;

	// --- ImGui�̕`�� ---
	
	ImGui::ColorEdit4(u8"MudSphereColor", m_MudColor);
	ImGui::ColorEdit4(u8"SeaColor", m_SeaColor);
	if (ImGui::Button("MudSphere"))
	{
		m_viewObj = OBJECT_MUDSPHERE;
	}
	else if (ImGui::Button("ConcreteCube"))
	{
		m_viewObj = OBJECT_CONCRETE;
	}
}
void SceneBump::Draw()
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pCamBuf = GetObj<ConstantBuffer>("CameraCB");
	ConstantBuffer* pColor = GetObj<ConstantBuffer>("Color");
	ConstantBuffer* pTexPos = GetObj<ConstantBuffer>("TexPos");

	MeshBuffer** ppMeshes;

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);

	// ���C�g�̏���ݒ�
	//DirectX::XMFLOAT4 lightDir(cosf(m_rad), -1.0f, sinf(m_rad), 0.0f);
	DirectX::XMFLOAT4 lightDir(1.0f, -1.0f, sinf(m_rad), 0.0f);
	pLight->Write(&lightDir);
	pLight->BindPS(0);

	DirectX::XMFLOAT4 camBuf[] = {
		DirectX::XMFLOAT4(pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z, 0.0f),
		DirectX::XMFLOAT4(10.0f, 30.0f, 0.0f, 0.0f)
	};
	pCamBuf->Write(&camBuf);
	pCamBuf->BindPS(1);

	// Unity����񃂃f���̕\��
	#pragma region draw unity model
	// �@���}�b�v�ݒ�
	Texture* pTex = GetObj<Texture>("NormalMap");
	SetTexturePS(pTex->GetResource(), 1);

	Model* pModel = GetObj<Model>("UnityModel");
	// pModel->Draw();

	// �o���v�}�b�v���f���̕\��
	pLight->BindVS(1);
	GetVS(VS_BUMP)->Bind();
	GetPS(PS_BUMPMAP)->Bind();
	pModel->Draw();
	/*MeshBuffer** ppMeshes = GetObj<MeshBuffer*>("UnityBump");
	for (int i = 0; i < pModel->GetMeshNum(); ++i)
	{
		ppMeshes[i]->Draw();
	}*/
	#pragma endregion

	// �`�悷��I�u�W�F�N�g�̐ݒ�
	#pragma region draw object
	switch (m_viewObj)
	{
		// �D�V�F�[�_�[
		case SceneBump::OBJECT_MUDSPHERE:
			// �D�̃��f���̕\��
			#pragma region draw mud sphere model
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("MudTex");
			SetTexturePS(pTex->GetResource(), 1);

			// �D�̕\��
			pModel = GetObj<Model>("MudSphereModel");
			// ���C�g�̐ݒ�
			pLight->BindVS(1);
			// �J���[�̐ݒ�
			pColor->Write(&m_MudColor);
			pColor->BindPS(2);

			GetVS(VS_MUD)->Bind();
			GetPS(PS_MUD)->Bind();

			ppMeshes = GetObj<MeshBuffer*>("MudSphereBump");
			for (int i = 0; i < pModel->GetMeshNum(); ++i)
			{
				ppMeshes[i]->Draw();
			}
			#pragma endregion 
			break;
		// �R���N���[�g�V�F�[�_�[
		case SceneBump::OBJECT_CONCRETE:
			// �D�̃��f���̕\��
			#pragma region draw concrete model
			// �@���}�b�v�ݒ�
			pTex = GetObj<Texture>("ConcreteTex");
			SetTexturePS(pTex->GetResource(), 1);

			// �R���N���[�g�̕\��
			pModel = GetObj<Model>("ConcreteModel");
			// ���C�g�̐ݒ�
			pLight->BindVS(1);
			// �J���[�̐ݒ�

			GetVS(VS_CONCRETE)->Bind();
			GetPS(PS_CONCRETE)->Bind();

			ppMeshes = GetObj<MeshBuffer*>("ConcreteBump");
			for (int i = 0; i < pModel->GetMeshNum(); ++i)
			{
				ppMeshes[i]->Draw();
			}
			#pragma endregion 
			break;
	}
	#pragma endregion
	
	// �n�ʃ��f���̕\��
	#pragma region draw ground model
	// �@���}�b�v�ݒ�
	pTex = GetObj<Texture>("SeaWaveTex");
	SetTexturePS(pTex->GetResource(), 1);

	// ���C�g�̐ݒ�
	DirectX::XMFLOAT4 sealightDir(cosf(m_seaLightRad), -1.0f, sinf(m_seaLightRad), 0.0f);
	pLight->Write(&sealightDir);
	pLight->BindVS(1);
	// �J���[�̐ݒ�
	pColor->Write(&m_SeaColor);
	pColor->BindPS(2);

	GetVS(VS_SEA)->Bind();
	GetPS(PS_SEA)->Bind();

	ppMeshes = GetObj<MeshBuffer*>("GroundBump");
	for (int i = 0; i < pModel->GetMeshNum(); ++i)
	{
		ppMeshes[i]->Draw();
	}
	#pragma endregion 

	
}

MeshBuffer** CreateBumpModel(Model* _pModel)
{
	//�o���v�p�̃��f������
	int meshNum = _pModel->GetMeshNum();
	MeshBuffer** _ppMesh = new MeshBuffer * [meshNum];
	for (int i = 0; i < meshNum; ++i)
	{
		MeshBuffer::Description desc = {};
		//���b�V���̒��_���ɍ��킹�āA�o���v�̒��_���m��
		int vtxNum = _pModel->GetMesh(i)->vertexNum;
		Model::Vertex* pVtx = _pModel->GetMesh(i)->pVertices;
		BumpVertex* pBumpVtx = new BumpVertex[vtxNum];
		for (int j = 0; j < vtxNum; ++j)
		{
			//�o���v�ɒ��_���R�s�[
			pBumpVtx[j].pos = pVtx[j].pos;
			pBumpVtx[j].normal = pVtx[j].normal;
			pBumpVtx[j].uv = pVtx[j].uv;
		}
		//�ʐ��ɍ��킹�āA�ڃx�N�g�����v�Z
		int idxNum = _pModel->GetMesh(i)->indexNum;
		unsigned int* pIdx = _pModel->GetMesh(i)->pIndices;
		int* tangentVtxCnt = new int[vtxNum];	//��̒��_�������̖ʂɗ��p����Ă���ꍇ
		ZeroMemory(tangentVtxCnt, sizeof(int) * vtxNum);
		for (int j = 0; j < idxNum; j += 3)
		{
			//�ʂ��\������C���f�b�N�X
			int idx0 = pIdx[j + 0];
			int idx1 = pIdx[j + 1];
			int idx2 = pIdx[j + 2];
			//�ڃx�N�g�����̊i�[��
			DirectX::XMFLOAT3* pTangents[] =
			{
				&pBumpVtx[idx0].tangent,
				&pBumpVtx[idx1].tangent,
				&pBumpVtx[idx2].tangent
			};
			//�ڃx�N�g���̌v�Z�ɕK�v�Ȋe�킶�傤�ق�
			DirectX::XMFLOAT3 v0(
				pBumpVtx[idx1].pos.x - pBumpVtx[idx0].pos.x,
				pBumpVtx[idx1].pos.y - pBumpVtx[idx0].pos.y,
				pBumpVtx[idx1].pos.z - pBumpVtx[idx0].pos.z
			);
			DirectX::XMFLOAT3 v1(
				pBumpVtx[idx2].pos.x - pBumpVtx[idx0].pos.x,
				pBumpVtx[idx2].pos.y - pBumpVtx[idx0].pos.y,
				pBumpVtx[idx2].pos.z - pBumpVtx[idx0].pos.z
			);
			DirectX::XMFLOAT2 t0(
				pBumpVtx[idx1].uv.x - pBumpVtx[idx0].uv.x,
				pBumpVtx[idx1].uv.y - pBumpVtx[idx0].uv.y
			);
			DirectX::XMFLOAT2 t1(
				pBumpVtx[idx2].uv.x - pBumpVtx[idx0].uv.x,
				pBumpVtx[idx2].uv.y - pBumpVtx[idx0].uv.y
			);
			/*
			|T.x T.y T.z|				1            |
			|           | = -------------------------|
			|B.x B.y B.z|   t0.x * t1.y - t1.x * t0.y|
			*/
			float t = 1.0f / (t0.x * t1.y - t1.x * t0.y);
			DirectX::XMVECTOR T = DirectX::XMVectorSet(
				t * (t1.y * v0.x - t0.y * v1.x),
				t * (t1.y * v0.y - t0.y * v1.y),
				t * (t1.y * v0.z - t0.y * v1.z), 0.0f
			);
			T = DirectX::XMVector3Normalize(T);
			//�ڃx�N�g���͍��Z���čŌ�ɕ��ς��o��
			for (int k = 0; k < 3; ++k)
			{
				DirectX::XMStoreFloat3(pTangents[k],
					DirectX::XMVectorAdd(T, DirectX::XMLoadFloat3(pTangents[k])));
				tangentVtxCnt[pIdx[j + k]]++;
			}
		}
		//�ڃx�N�g���̕��ς��Z�o
		for (int j = 0; j < vtxNum; ++j)
		{
			pBumpVtx[j].tangent.x /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.y /= tangentVtxCnt[j];
			pBumpVtx[j].tangent.z /= tangentVtxCnt[j];
		}
		//���f���f�[�^�쐬
		desc.pVtx = pBumpVtx;
		desc.vtxSize = sizeof(BumpVertex);
		desc.vtxCount = vtxNum;
		desc.pIdx = pIdx;
		desc.idxSize = sizeof(unsigned int);
		desc.idxCount = idxNum;
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_ppMesh[i] = new MeshBuffer(desc);

		//�s�v�ȃf�[�^�폜
		delete[] pBumpVtx;
		delete[] tangentVtxCnt;
	}

	return _ppMesh;
}