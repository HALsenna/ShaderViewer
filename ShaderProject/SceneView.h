#ifndef __SCENE_VIEW_H__
#define __SCENE_VIEW_H__

#include "SceneBase.hpp"

enum SceneKind
{
	SCENE_NONE,
	SCENE_TITLE,	// �^�C�g���V�[��
	SCENE_SELECT,	// �ݒ�V�[��
	SCENE_GAME,		// �Q�[���V�[��
	SCENE_MAX
};


class SceneView : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SceneChange(SceneKind scene);
	void LoadScene(int index);

private:
	int m_index;
};

#endif // __SCENE_VIEW_H__