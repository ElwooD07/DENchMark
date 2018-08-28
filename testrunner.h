#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>

enum TestMethod
{
    CPU = 0,
    Memory,
    CPUandMemory
};

struct ThreadInfo
{
    ThreadInfo(volatile bool& stop_, int64_t complexity_)
        : stop(stop_)
        , complexity(complexity_)
    {  }

    ThreadInfo& operator=(const ThreadInfo& right)
    {
        stop = right.stop;
        complexity = right.complexity;
        start = right.start;
        finish = right.finish;
        return *this;
    }

    volatile bool& stop;
    int64_t complexity;
    int64_t start = 0;
    int64_t finish = 0;
};

using ThreadsInfo = std::vector<ThreadInfo>;

class TestRunner
{
public:
    TestRunner();
    void Go(size_t numberOfThreads, TestMethod method, int64_t complexity);
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
