#pragma once

#include <vector>
#include <iostream>

class CTimer
{
private:
public:
	// タイマーの状態
	enum TIMERSTATE
	{
		TIMER_NOTACTIVE = 0,	// 起動していない
		TIMER_ACTIVE,			// タイマー実行中
		TIMER_FIN,				// タイマー終了
	};

	// タイマーの情報
	struct TIMERINFO
	{
		float fWaitSec;		// 待機時間
		float fCurrentTime;	// 現在時間
		TIMERSTATE state;
		const char* szName;	// 名前
	};
	
public:
	CTimer();
	~CTimer();
	void Update();
	void Uninit();
	void Set(float seconds, const char* szName);		// 遅延処理
	bool isTimerFin(const char* szName);
	void Reset(const char* szName);
private:
	TIMERINFO checkName(const char* szName);


private:
	std::vector<TIMERINFO> m_TimerList;		// タイマーのリスト
};
