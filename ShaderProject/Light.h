#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class CLight
{
public:
	// ���z��
	struct DirectionalLight
	{
		XMFLOAT3 pos;			// �����̍��W
		XMFLOAT3 direction;		// ���̕���
		XMFLOAT3 color;			// ���̐F
		bool	 isLight;		// �_�����邩�ǂ���
	};

	// �|�C���g���C�g
	struct PointLight
	{
		XMFLOAT3 pos;			// �����̍��W
		float	 range;			// ���͈̔�
		XMFLOAT3 color;			// ���̐F
		bool	 isLight;		// �_�����邩�ǂ���
	};
	
	// �X�|�b�g���C�g
	//struct SpotLight
	//{
	//	XMFLOAT3 pos;		// �����̍��W
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
	DirectionalLight m_DirLight;				// ���z��
	std::vector<PointLight> m_PointLightList;	// �|�C���g���C�g
};
