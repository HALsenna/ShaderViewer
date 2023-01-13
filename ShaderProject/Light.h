#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class CLight
{
public:
	// 太陽光
	struct DirectionalLight
	{
		XMFLOAT3 pos;			// 光源の座標
		XMFLOAT3 direction;		// 光の方向
		XMFLOAT3 color;			// 光の色
		bool	 isLight;		// 点灯するかどうか
	};

	// ポイントライト
	struct PointLight
	{
		XMFLOAT3 pos;			// 光源の座標
		float	 range;			// 光の範囲
		XMFLOAT3 color;			// 光の色
		bool	 isLight;		// 点灯するかどうか
	};
	
	// スポットライト
	//struct SpotLight
	//{
	//	XMFLOAT3 pos;		// 光源の座標
	//	float 
	//};
public:
	CLight();
	~CLight();
	void Init();
	void Update();
	void Draw();
	void SetSkyColor(XMFLOAT3 color);
	XMFLOAT3 GetSkyColor();
	void SetSkyLightDir(XMFLOAT3 direction);
	XMFLOAT3 GetSkyLightDir();
private:
	DirectionalLight m_DirLight;				// 太陽光
	std::vector<PointLight> m_PointLightList;	// ポイントライト
};
