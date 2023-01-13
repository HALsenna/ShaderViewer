#pragma once
#include "Geometory.h"
#include "DirectX.h"
#include "UIList.h"
#include "Texture.h"

using namespace DirectX;


// UI�`��ɕK�v�ȏ��
struct UIINFO
{
	XMFLOAT3 pos;
	XMFLOAT2 size;
	XMFLOAT4 color;
	Texture* pTex;
};

enum STAGINGSTATE
{
	STAGING_FADEIN = 0,		// �t�F�[�h�C��
	STAGING_FADEOUT,		// �t�F�[�h�A�E�g
	STAGING_FLASH,			// �t���b�V��
	STAGING_DISSOLOVE,		// �f�B�]���u���o
	STAGING_NONE,			// ���o����
	
	STAGING_MAX
};

class CUI
{
public:
	CUI(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, Texture* pTex);
	~CUI();
	void Update();
	void Draw();
	void Fade(STAGINGSTATE state, float fadesec);
	void Flash(float flashSpeed);
	void Dissolve(float dissolvesec, Texture* pTex);
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 GetPos();
private:
	void Init();
private:
	UIINFO m_uiInfo;
	CGeometory* m_pGeometory;
	STAGINGSTATE m_state;		// �t�F�[�h�̏��
	float m_fFadeSec;			// �`��J�n/�I�����Ƀt�F�[�h���邩�ǂ���
	float m_fFlashSpeed;		// �_�ŃX�s�[�h
	float m_fTime;
	float m_dissolveRate;
	float m_dissolveTime;
	bool m_bFlashFadeIn;	
	Texture* m_pTex;			// �e�N�X�`��
	Texture* m_pDissolveTex;	// �f�B�]���u�e�N�X�`��
};
