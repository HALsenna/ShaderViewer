#pragma once
#include <DirectXMath.h>
#include "ShaderList.h"
#include "Model.h"
#include "SceneObject.h"
using namespace DirectX;

class CMeat_Setting
{
public:
	CMeat_Setting(SCENEOBJECTINFO info);
	~CMeat_Setting();
	void Init();
	void Update();
	void Draw();
private:
	SCENEOBJECTINFO m_ObjectInfo;		// オブジェクト情報
};
