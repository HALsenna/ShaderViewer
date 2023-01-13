#pragma once
#include "Geometory.h"
#include "Texture.h"

class SceneFade
{
public:
	enum FADESTATE
	{
		FADE_IN = 0,		// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
		FADE_NONE,			// �������Ă��Ȃ����
		FADE_FIN,		// �t�F�[�h�I��		
		FADE_MAX,
	};
public:
	SceneFade();
	~SceneFade();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetFade(FADESTATE state, float seconds );
	void Reset();
	bool IsFadeFin();
private:
	CGeometory* pGeometory;
	Texture* m_pFadeTex;		// �t�F�[�h�p�̃e�N�X�`��
	FADESTATE m_state;
	float m_fCurrentTime;		// ���݂̃J�E���g
	float m_fTexAlpha;			// �e�N�X�`���`��A���t�@�l
	float m_fFadeSec;			// �t�F�[�h����
};
