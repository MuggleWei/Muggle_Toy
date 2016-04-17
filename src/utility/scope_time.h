#ifndef __MUGGLE_SCOPE_TIME_H__
#define __MUGGLE_SCOPE_TIME_H__

#include "base/base.h"
#include "utility/timer.h"
#include <string>
#include <vector>

#if ENABLE_SCOPE_TIME_STATICSTICS
#define SCOPE_TIME_COUNT(str) ScopeTime str##RECORD_TIME_VAR(#str, __FILE__, __LINE__)
#define SCOPE_TIME_RESET ScopeTimeManager::Instance()->Reset()
#else
#define SCOPE_TIME_COUNT(str)
#define SCOPE_TIME_RESET
#endif

NS_MUGGLE_BEGIN

class ScopeTimeData
{
public:
    std::string id_str_;
    int         level_;
    double      elapsed_time_;
};

class ScopeTimeManager
{
public:
    static ScopeTimeManager* Instance()
    {
        if (singleton == nullptr)
        {
            singleton = new ScopeTimeManager;
        }
        return singleton;
    }
    static void DestroyInstance()
    {
        SAFE_DELETE(singleton);
    }
    static ScopeTimeManager* singleton;

    void Reset();
    void Print();

public:
    std::vector<ScopeTimeData>  datas_;
    std::vector<std::string>    record_;
};

class ScopeTime
{
public:
    ScopeTime(const char* name, const char* file_name, int line);
    ~ScopeTime();

private:
    Timer       timer_;
    std::string id_str_;
    int         level_;

    static int s_next_level;
};

NS_MUGGLE_END

#endif