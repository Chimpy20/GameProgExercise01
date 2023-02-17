#include "GameProgExercise01.h"
#include "Utils\Timers.h"

namespace utils
{

#ifdef _DEBUG
const float Timers::MaxDebugFrameTime = 1.0f; // When debugging, don't have a huge frame time when stepping over code
#endif

LARGE_INTEGER Timers::m_frequency = { 0 };
LARGE_INTEGER Timers::m_previousFrameTime = { 0 };
LARGE_INTEGER Timers::m_lastFrameTime = { 0 };
float Timers::m_frameTime = 0.0f;
LARGE_INTEGER Timers::m_timerStart = { 0 };

Timers::Timers()
{
}

Timers::~Timers()
{
}

void Timers::InitialiseTimers()
{
	QueryPerformanceFrequency( &m_frequency );
}

void Timers::UpdateFrameTimer()
{
	QueryPerformanceCounter( &m_lastFrameTime );
	LARGE_INTEGER frameTime;
	frameTime.QuadPart = m_lastFrameTime.QuadPart - m_previousFrameTime.QuadPart;
	m_frameTime = static_cast<float>( frameTime.QuadPart ) / static_cast<float>( m_frequency.QuadPart );
	m_previousFrameTime = m_lastFrameTime;

#ifdef _DEBUG
	if( m_frameTime > MaxDebugFrameTime )
		m_frameTime = MaxDebugFrameTime;
#endif
}

void Timers::StartTimer()
{
	QueryPerformanceCounter( &m_timerStart );
}

void Timers::EndTimer( const CHAR* tag )
{
	LARGE_INTEGER timerEnd;
	QueryPerformanceFrequency( &timerEnd );
	const float duration = static_cast<float>( timerEnd.QuadPart - m_timerStart.QuadPart ) * 1000.0f / static_cast<float>( m_frequency.QuadPart );
	io::OutputMessage( "%s took %.4fms\n", tag, duration );
}

}