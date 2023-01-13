#pragma once

#include <vector>
#include <iostream>

class CTimer
{
private:
public:
	// �^�C�}�[�̏��
	enum TIMERSTATE
	{
		TIMER_NOTACTIVE = 0,	// �N�����Ă��Ȃ�
		TIMER_ACTIVE,			// �^�C�}�[���s��
		TIMER_FIN,				// �^�C�}�[�I��
	};

	// �^�C�}�[�̏��
	struct TIMERINFO
	{
		float fWaitSec;		// �ҋ@����
		float fCurrentTime;	// ���ݎ���
		TIMERSTATE state;
		const char* szName;	// ���O
	};
	
public:
	CTimer();
	~CTimer();
	void Update();
	void Uninit();
	void Set(float seconds, const char* szName);		// �x������
	bool isTimerFin(const char* szName);
	void Reset(const char* szName);
private:
	TIMERINFO checkName(const char* szName);


private:
	std::vector<TIMERINFO> m_TimerList;		// �^�C�}�[�̃��X�g
};
