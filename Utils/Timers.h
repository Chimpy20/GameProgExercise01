#pragma once

namespace utils
{

class Timers
{
#ifdef _DEBUG
	static const float MaxDebugFrameTime;
#endif

public:
	Timers();
	~Timers();

	static void				InitialiseTimers();
	static void				UpdateFrameTimer();
	inline static float		GetFrameTime()
	{
		return m_frameTime;
	}
	static void				StartTimer();
	static void				EndTimer( const CHAR* tag );

private:
	static LARGE_INTEGER	m_frequency;
	static LARGE_INTEGER	m_previousFrameTime, m_lastFrameTime;
	static float			m_frameTime;
	static LARGE_INTEGER	m_timerStart;
};

} // namespace utils