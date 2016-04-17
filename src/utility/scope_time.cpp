#include "scope_time.h"

NS_MUGGLE_BEGIN

// ScopeTimeManager
ScopeTimeManager* ScopeTimeManager::singleton = nullptr;

void ScopeTimeManager::Reset()
{
    record_.clear();

    if (datas_.size() > 0)
    {
        int size = datas_.size();

        double sum = datas_[datas_.size() - 1].elapsed_time_;

        for (int index = (int)datas_.size() - 1; index >= 0;)
        {
            int pre_index = index - 1;
            if (pre_index >= 0 && datas_[pre_index].level_ == datas_[index].level_)
            {
                pre_index--;
            }

            for (int i = pre_index + 1; i <= index; ++i)
            {
                std::string blank_str = "";
                for (int j = 0; j < datas_[i].level_; ++j)
                {
                    blank_str += " ";
                }

                char buf[2048];
                
                double percent = 100 * datas_[i].elapsed_time_ / sum;
                sprintf(buf, "%s%s - %f (%f%%)", 
                    blank_str.c_str(), 
                    datas_[i].id_str_.c_str(), 
                    datas_[i].elapsed_time_, 
                    percent);
                record_.push_back(buf);
            }

            index = pre_index;
        }

        MASSERT(record_.size() == datas_.size());
        datas_.clear();
    }
}
void ScopeTimeManager::Print()
{
    for (auto it = record_.begin(); it != record_.end(); ++it)
    {
        MLOG("%s\n", it->c_str());
    }
}

// ScopeTime
int ScopeTime::s_next_level = 0;

ScopeTime::ScopeTime(const char* name, const char* file_name, int line)
{
    id_str_ = id_str_ + " " + name + " " + file_name;
    char buf[256];
    sprintf(buf, " %d", line);
    id_str_ += buf;

    level_ = s_next_level++;

    timer_.Start();
}
ScopeTime::~ScopeTime()
{
    timer_.End();
    double elapsed_time = timer_.GetElapsedMilliseconds();

    std::vector<ScopeTimeData>& datas = ScopeTimeManager::Instance()->datas_;
    if (datas.size() > 0 && datas[datas.size() - 1].id_str_ == id_str_ && datas[datas.size() - 1].level_ == level_)
    {
        datas[datas.size() - 1].elapsed_time_ += elapsed_time;
    }
    else
    {
        ScopeTimeData data = {id_str_, level_, elapsed_time};
        datas.push_back(data);
    }

    --s_next_level;
    MASSERT_MSG(s_next_level >= 0, "s_next_level must be positive");
}

NS_MUGGLE_END