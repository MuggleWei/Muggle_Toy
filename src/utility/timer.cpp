#include "timer.h"

NS_MUGGLE_BEGIN

const double Timer::fps_60_delta = 16.666667;
const double Timer::fps_30_delta = 33.333333;
const double Timer::fps_15_delta = 66.666667;
const double Timer::fps_7_delta = 142.85714;

HightPrecisionTime Timer::s_init_time;
double Timer::s_delta = 0.0;
double Timer::s_fixed_delta = fps_60_delta;
double Timer::s_lock_delta = fps_60_delta;
double Timer::s_current_time = 0.0;

#if MG_PLATFORM_WINDOWS

    void Timer::Start()
    {
        QueryPerformanceCounter(&m_start);
    }

    void Timer::End()
    {
        QueryPerformanceCounter(&m_end);
    }

    double Timer::GetElapsedMilliseconds()
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return (m_start.QuadPart - m_end.QuadPart) * 1000.0 / freq.QuadPart;
    }

    double Timer::GetMilliseconds()
    {
        LARGE_INTEGER freq, cur;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&cur);
        return (cur.QuadPart - s_init_time.QuadPart) * 1000.0 / freq.QuadPart;
    }
    void Timer::Reset()
    {
        QueryPerformanceCounter(&s_init_time);
    }

#else
    void Timer::Start()
    {
        gettimeofday(&m_start, NULL);
    }

    void Timer::End()
    {
        gettimeofday(&m_end, NULL);
    }

    double Timer::GetElapsedMilliseconds()
    {
        return (m_end.tv_sec - m_start.tv_sec) * 1000.0 + (m_end.tv_usec - m_start.tv_usec) / 1000.0;
    }

    double Timer::GetMilliseconds()
    {
        struct timeval cur;
        gettimeofday(&cur, NULL);
        return (cur.tv_sec - s_init_time.tv_sec) * 1000.0 + (cur.tv_usec - s_init_time.tv_usec) / 1000.0;
    }
    void Timer::Reset()
    {
        gettimeofday(&s_init_time, NULL);
    }

#endif

void Timer::Tick()
{
	double last_time = s_current_time;
	s_current_time = GetMilliseconds();
	s_delta = s_current_time - last_time;
}
double Timer::DeltaTime()
{
	return s_delta;
}
double Timer::FixedDeltaTime()
{
	return s_fixed_delta;
}

double Timer::GetLockDeltaTime()
{
	return s_lock_delta;
}
bool Timer::IsLockDelta()
{
	return s_lock_delta > 0.0;
}

void Timer::LockDelta(double lock_delta)
{
	s_lock_delta = lock_delta;
}

double Timer::GetLastTickTime()
{
	return s_current_time;
}

NS_MUGGLE_END