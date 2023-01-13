#pragma once
#include "Model.h"
#include "CameraBase.h"
#include "ConstantBuffer.h"

#define TIME_SKYCHANGE		(3.0f)

enum SKYKIND
{
	SKY_NOON = 0,	// ’‹
	SKY_EVENING,	// —[•û
	SKY_NIGHT,		// –é
	SKY_MAX
};
using namespace DirectX;
class CSkyDome : public Model
{
public:
	CSkyDome(Model* pModel[SKY_MAX]);
	~CSkyDome();
	void Init();
	void Update();
	void Draw();
	void SetCamera(CameraBase* camera, ConstantBuffer* cb);
	void SetSkyKind(SKYKIND kind);
	SKYKIND GetSkyKind();
private:
	float m_fTime;
	Model* m_pModel[SKY_MAX];
	CameraBase* m_camera;
	SKYKIND m_skyKind;
	ConstantBuffer* m_cb;
	ConstantBuffer* m_alpha;
};
