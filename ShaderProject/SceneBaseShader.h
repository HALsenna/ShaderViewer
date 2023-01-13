#ifndef __SCENE_BASESHADER_H__
#define __SCENE_BASESHADER_H__

#include "SceneBase.hpp"
#include "ConstantBuffer.h"
#include "Model.h"
#include "CameraBase.h"
#include <vector>
#include "ObjectsList.h"
#include "CameraDepth.h"
#include "Food.h"

using namespace DirectX;
using namespace std;

class SceneBaseShader : public SceneBase
{
public:
	enum KINDSHADER
	{
		SHADER_OBJECT = 0,		// �I�u�W�F�N�g��\�����邾��
		SHADER_DIRECTIONLIGHT,	// �f�B���N�V�������C�g
		SHADER_MAX,
	};

	enum IMGUIFLG
	{
		CB_DIRTIONLIGHT = 0,	// �f�B���N�V�������C�g(�`�F�b�N�{�b�N�X)
		IMGUI_MAX,
	};

private:
	struct DirectionLight
	{
		XMFLOAT4 lightDir;		// ���C�g�̕���
		XMFLOAT4 lightColor;	// ���C�g�̃J���[
	};

	enum BUMPMODEL
	{
		BUMPMODEL_ROCK,
		BUMPMODEL_TENT,
		BUMPMODEL_MAX
	};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void InitLight();
	void InitModel();
	void SetDirectionLight();	// �g�U���̐ݒ�
	void SetSpecular();			// ���ʔ��ˌ��̐ݒ�
	void SetCamera(CameraBase* _pCamera, ConstantBuffer* _pCameraBuf);			// �J����
	void DrawDirectionLight(ConstantBuffer* _pLight);	// �g�U���̕`��
	void DrawSpecular(ConstantBuffer* _pLight);	// �g�U���̕`��
	void DrawField(XMMATRIX worldMatrix, int index, XMFLOAT4X4 mat, ConstantBuffer* _pWVP, Texture* _pTex,
		ConstantBuffer* _pLight, CameraBase* _pCamera, ConstantBuffer* _pCamBuf);

private:
	float m_rad;
	float m_seaLightRad;
	float m_MudColor[4] = { 0.45f, 0.3f, 0.18f, 1.0f };
	float m_SeaColor[4] = { 0.37f, 0.5f, 0.87f, 1.0f };
	float m_rate;
	XMVECTOR vecCamera;

	bool m_isView[SHADER_MAX];		// �V�F�[�_�[�\������
	bool m_ImGUIFlg[IMGUI_MAX];		// ImGUI�p�t���O

	DirectionLight m_directionLight;	// �f�B���N�V�������C�g
	PointLight* m_PointLight;			// �|�C���g���C�g
	float m_time;

	//OBJECTLIST m_sceneObj;
	Model* m_pSceneModel[OBJ_MAX];			// ���f�����
	MeshBuffer** m_pBumpModel[BUMPMODEL_MAX];			// ���f�����
	CameraDepth* m_cameraDepth;

	CFood* pFoodManager;
};

#endif // __SCENE_BASESHADER_H__
