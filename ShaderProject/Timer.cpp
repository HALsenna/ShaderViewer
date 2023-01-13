#include "Timer.h"
#include "WinDialog.h"


CTimer::CTimer()
{
	
}

CTimer::~CTimer()
{

}

void CTimer::Update()
{
	for (int i = 0; i < m_TimerList.size(); ++i)
	{
		// �^�C�}�[���쒆�̏ꍇ
		if (m_TimerList[i].state == TIMER_ACTIVE)
		{
			m_TimerList[i].fCurrentTime += 1.0f / 60.0f;

			// ���ԂɂȂ�����^�C�}�[�I��
			if (m_TimerList[i].fCurrentTime > m_TimerList[i].fWaitSec)
			{
				m_TimerList[i].state = TIMER_FIN;
			}
		}
	}
}

// �I������
void CTimer::Uninit()
{
	m_TimerList.clear();
}

// �^�C�}�[�̐ݒ�
void CTimer::Set(float seconds, const char* szName)
{
	TIMERINFO timer;		// �^�C�}�[���i�[�p�ϐ�
	timer.fCurrentTime = 0.0f;
	timer.fWaitSec = seconds;
	timer.state = TIMER_ACTIVE;
	timer.szName = szName;
	m_TimerList.push_back(timer);		// �^�C�}�[�����Z�b�g
}

// �^�C�}�[�I���ʒm
bool CTimer::isTimerFin(const char* szName)
{
	//return checkName(szName).state == TIMER_FIN;
	if (checkName(szName).state == TIMER_FIN)
		return true;
	else
		return false;
}

// �^�C�}�[�̖��O�m�F
CTimer::TIMERINFO CTimer::checkName(const char* szName)
{
	for (int i = 0; i < m_TimerList.size(); ++i)
	{
		if (m_TimerList[i].szName == szName)
			return m_TimerList[i];
	}

	// ���O����v���Ȃ������ꍇ�̃G���[����
	DialogError(STACK_DEFAULT + 1, "%s��������܂���ł���", szName);
}

// �^�C�}�[���Z�b�g
void CTimer::Reset(const char* szName)
{
	for (int i = 0; i < m_TimerList.size(); ++i)
	{
		if (m_TimerList[i].szName == szName)
		{
			m_TimerList[i].fCurrentTime = 0.0f;
			m_TimerList[i].fWaitSec = 0.0f;
			m_TimerList[i].state = TIMER_NOTACTIVE;
			return;
		}
	}

	// ���O����v���Ȃ������ꍇ�̃G���[����
	DialogError(STACK_DEFAULT + 1, "%s��������܂���ł���", szName);
}


