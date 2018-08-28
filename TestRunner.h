#pragma once
#include "TestInfo.h"
#include <Windows.h>
#include <mutex>

using ThreadsInfo = std::vector<ThreadInfo>;

class TestRunner
{
public:
    TestRunner();
    void Go(const TestParameters& parameters);
    bool IsRunning() const;
    void Stop();

    const ThreadsInfo& GetThreadsInfo() const;

private:
    void TerminateThreads();

private:
    std::vector<HANDLE> m_threads;
    std::vector<ThreadInfo> m_threadsInfo;
    std::mutex m_startStopMutex;
    volatile bool m_stop;
};
