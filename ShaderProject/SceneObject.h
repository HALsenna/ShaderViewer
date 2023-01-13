#pragma once
#include <DirectXMath.h>
#include "ShaderList.h"
#include "Model.h"

struct SCENEOBJECTINFO
{
	const char* name;			// ���O
	const char* fileName;		// �Ǎ��t�@�C���̖��O
	DirectX::XMFLOAT3 pos;		// ���W
	DirectX::XMFLOAT3 rot;		// ��]
	XMFLOAT3 size;					// �T�C�Y
	VertexShaderKind vs;	// ���_�V�F�[�_�[
	PixelShaderKind ps;	// �s�N�Z���V�F�[�_�[
	Model* pModel;				// ���f���f�[�^
};
