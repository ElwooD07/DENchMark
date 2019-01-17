#pragma once
#include "TestInfo.h"

using ThreadsInfo = std::vector<ThreadInfo>;

class TestRunnerBase
{
public:
    TestRunnerBase();
    virtual void Go(const TestParameters& parameters) = 0;
    virtual bool IsRunning() const = 0;
    virtual void Stop() = 0;

    const ThreadsInfo& GetThreadsInfo() const;

protected:
    volatile bool m_stop;
    std::vector<ThreadInfo> m_threadsInfo;
};
