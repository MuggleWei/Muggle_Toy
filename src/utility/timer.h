#ifndef __MUGGLE_TIMER_H__
#define __MUGGLE_TIMER_H__

#include "base/base.h"

#ifdef _WIN32
#include <windows.h>
typedef LARGE_INTEGER HightPrecisionTime;
#else
#include <sys/time.h>
typedef struct timeval HightPrecisionTime;
#endif

NS_MUGGLE_BEGIN

class MG_DLL Timer
{
public:
    Timer() : m_start(), m_end() {}

    void Start();
    void End();
    double GetElapsedMilliseconds();

	// static 
	static void Tick();
	static double DeltaTime();
	static double FixedDeltaTime();

	static double GetLockDeltaTime();
	static bool IsLockDelta();

	/*
	 *  whether or not lock delta time
	 *  @lock_delta: if lock_delta <= 0.0, not lock, otherwise, lock delta time
	 */
	static void LockDelta(double lock_delta);

	static double GetLastTickTime();

	/*
	 *  Get elapsed from last reset in milliseconds
	 */
    static double GetMilliseconds();
    static void Reset();

protected:
	HightPrecisionTime m_start;
	HightPrecisionTime m_end;

    static HightPrecisionTime s_init_time;
    static double s_delta;
    static double s_fixed_delta;
    static double s_lock_delta;
	static double s_current_time;

public:
    const static double fps_60_delta;
    const static double fps_30_delta;
    const static double fps_15_delta;
    const static double fps_7_delta;
};

NS_MUGGLE_END

#endif