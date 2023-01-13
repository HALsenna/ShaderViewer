#ifndef __SCENE_BUMP_H__
#define __SCENE_BUMP_H__

#include "SceneBase.hpp"

class SceneBump : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	float m_rad;
	float m_seaLightRad;
	float m_MudColor[4] = { 0.45f, 0.3f, 0.18f, 1.0f };
	float m_SeaColor[4] = { 0.37f, 0.5f, 0.87f, 1.0f };

	enum OBJECT
	{
		OBJECT_MUDSPHERE = 0,		// 泥シェーダー
		OBJECT_CONCRETE,			// コンクリート
		OBJECT_MAX,
	};

	OBJECT m_viewObj;
};

#endif // __SCENE_SHADING_H__
