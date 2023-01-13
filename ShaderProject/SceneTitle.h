#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneBase.hpp"
#include "ShaderList.h"
#include "Model.h"
#include <vector>
#include <DirectXMath.h>
#include "UIObj.h"

using namespace DirectX;
// �I�u�W�F�N�g���
struct TitleObject
{
	const char* name;		// ���O
	const char* fileName;	// �Ǎ��t�@�C���̖��O
	XMFLOAT3 pos;			// �������̍��W
	XMFLOAT3 rot;			// �������̉�]
	float size;				// �������̑傫��
	VertexShaderKind vs;	// ���_�V�F�[�_�[
	PixelShaderKind ps;		// �s�N�Z���V�F�[�_�[
	Model* pModel;			// ���f�����
};

class SceneTitle : public SceneBase
{
// �����o�֐�
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetObjInfo(TitleObject obj);

private:
	//void CreateModel();

// �����o�ϐ�
private:
	std::vector<TitleObject> m_sceneObj;		// �V�[���I�u�W�F�N�g
public:
	SceneTitle* m_pTitleScene;
};

#endif // __SCENE_SHADING_H__