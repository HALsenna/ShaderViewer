#ifndef __SCENE_SELECT_H__
#define __SCENE_SELECT_H__

#include "SceneBase.hpp"
#include "UIObj.h"			// UI�`��
#include "UIList.h"			// UI�̎��
#include "SettingObjectsList.h"		// �I�u�W�F�N�g�̎��
#include "ShaderList.h"
#include <vector>
#include "Model.h"
#include "SceneObject.h"
#include "SettingSceneLight.h"		// ����

class SceneSelect : public SceneBase
{
	// �����o�֐�
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void Init3D();		// 3D�̏�����
	void InitUI();		// UI�̏�����
	void InitModel();	// ���f���̏�����
	void CreateModel();	// ���f���̐���
	void InitLight();	// �����̏�����
	void CreateObject();	// �I�u�W�F�N�g�̐���
	void SetShader(SELECTSCENE_OBJECT kind);	// �V�F�[�_�[���̃Z�b�g
	void UpdateKeyInput();

	// �����o�ϐ�
private:
	CUI* m_pUIObj[SELECT_MAX];
	UIINFO m_aUIInitData[SELECT_MAX];			// UI�̏����f�[�^
	std::vector<SCENEOBJECTINFO> m_ObjInitData;	// �V�[���I�u�W�F�N�g�̏����f�[�^
	std::vector<Model*> m_pObjData;				// �V�[���I�u�W�F�N�g�̃f�[�^
	CSelectSceneLight* m_pSceneLight;					// �V�[���̊���
	//UIINFO  m_aUIInitData[SELECT_MAX];		// UI�̏����f�[�^
};

#endif // __SCENE_SHADING_H__