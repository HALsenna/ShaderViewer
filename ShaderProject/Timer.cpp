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
		// タイマー動作中の場合
		if (m_TimerList[i].state == TIMER_ACTIVE)
		{
			m_TimerList[i].fCurrentTime += 1.0f / 60.0f;

			// 時間になったらタイマー終了
			if (m_TimerList[i].fCurrentTime > m_TimerList[i].fWaitSec)
			{
				m_TimerList[i].state = TIMER_FIN;
			}
		}
	}
}

// 終了処理
void CTimer::Uninit()
{
	m_TimerList.clear();
}

// タイマーの設定
void CTimer::Set(float seconds, const char* szName)
{
	TIMERINFO timer;		// タイマー情報格納用変数
	timer.fCurrentTime = 0.0f;
	timer.fWaitSec = seconds;
	timer.state = TIMER_ACTIVE;
	timer.szName = szName;
	m_TimerList.push_back(timer);		// タイマー情報をセット
}

// タイマー終了通知
bool CTimer::isTimerFin(const char* szName)
{
	//return checkName(szName).state == TIMER_FIN;
	if (checkName(szName).state == TIMER_FIN)
		return true;
	else
		return false;
}

// タイマーの名前確認
CTimer::TIMERINFO CTimer::checkName(const char* szName)
{
	for (int i = 0; i < m_TimerList.size(); ++i)
	{
		if (m_TimerList[i].szName == szName)
			return m_TimerList[i];
	}

	// 名前が一致しなかった場合のエラー処理
	DialogError(STACK_DEFAULT + 1, "%sが見つかりませんでした", szName);
}

// タイマーリセット
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

	// 名前が一致しなかった場合のエラー処理
	DialogError(STACK_DEFAULT + 1, "%sが見つかりませんでした", szName);
}


