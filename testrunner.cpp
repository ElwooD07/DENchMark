#include "testrunner.h"
#include <Windows.h>
#include <assert.h>

namespace
{
    inline ThreadInfo& GetThreadInfo(PVOID parameters)
    {
        return *(reinterpret_cast<ThreadInfo*>(parameters));
    }

    DWORD WINAPI ThreadProcCPU(PVOID parameters)
    {
        ThreadInfo& info = GetThreadInfo(parameters);
        info.start = ::GetTickCount64();

        size_t primesFound = 1; // 2 is already counted
        for (int64_t i = 2; i < info.complexity && !info.stop; ++i)
        {
            bool isPrime = false;
            for (int64_t j = 2; j < i && !info.stop; ++j)
            {
                if (i % j == 0)
                {
                    break;
                }
            }

            if (isPrime)
            {
                ++primesFound;
            }
        }

        info.finish = ::GetTickCount64();
        return 0;
    }

    DWORD WINAPI ThreadProcMemory(PVOID parameters)
    { // Implements Erythrophene Sieve
        ThreadInfo& info = GetThreadInfo(parameters);
        std::vector<size_t> grid(info.complexity + 1);
        info.start = ::GetTickCount64();

        size_t primes = 0;
        const size_t gridSize = grid.size();
        for (int i = 0; i < gridSize && !info.stop; i++)
        {
            grid[i] = i;
        }

        for (size_t p = 2; p < gridSize && !info.stop; p++)
        {
            if (grid[p] != 0)
            {
                ++primes;
                for (size_t j = p * p; j < gridSize; j += p)
                { // Mark not prime numbers
                    grid[j] = 0;
                }
            }
        }

        info.finish = ::GetTickCount64();
        return 0;
    }

    DWORD WINAPI ThreadProcCPUandMemory(PVOID parameters)
    {
        GetThreadInfo(parameters).finish = 100500;
        // TODO
        return 0;
    }

    inline LPTHREAD_START_ROUTINE SelectThreadProcForTestMethod(TestMethod method)
    {
        switch(method)
        {
        case CPU:
            return &ThreadProcCPU;
        case Memory:
            return &ThreadProcMemory;
        case CPUandMemory:
            return &ThreadProcCPUandMemory;
        default:
            throw std::runtime_error("Unknown test method");
        }
    }

    double GetTestMethodComplexityRatio(TestMethod method)
    {
        switch(method)
        {
        case CPU:
            return 1;
        case Memory:
            return 855;
        case CPUandMemory:
            return 1; // TODO
        default:
            assert("Unknown test method");
            return 1;
        }
    }
}

TestRunner::TestRunner()
    : m_stop(false)
{
    if (!::SetPriorityClass(::GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
    {
        throw std::runtime_error("Can't set process priority. Try to launch this application with Administrative privilegies");
    }
}

void TestRunner::Go(size_t numberOfThreads, TestMethod method, int64_t complexity)
{
    std::unique_lock<std::mutex> lock(m_startStopMutex);

    if (IsRunning())
    {
        return;
    }

    complexity = static_cast<int64_t>(complexity * GetTestMethodComplexityRatio(method));

    m_stop = false;
    m_threads.assign(numberOfThreads, NULL);
    m_threadsInfo.assign(numberOfThreads, ThreadInfo(m_stop, complexity));

    LPTHREAD_START_ROUTINE threadProc = SelectThreadProcForTestMethod(method);
    for (size_t i = 0; i < numberOfThreads; ++i)
    {
        m_threads[i] = ::CreateThread(NULL, 0, threadProc, &(m_threadsInfo[i]), CREATE_SUSPENDED, NULL);
        if (m_threads[i] == NULL)
        {
            TerminateThreads();
            throw std::runtime_error("Unable to create thread");
        }
    }

    for (HANDLE thread : m_threads)
    {
        ::ResumeThread(thread);
    }
}

bool TestRunner::IsRunning() const
{
    for (const ThreadInfo& threadInfo : m_threadsInfo)
    {
        if (threadInfo.finish == 0)
        {
            return true;
        }
    }
    return false;
}

void TestRunner::Stop()
{
    std::unique_lock<std::mutex> lock(m_startStopMutex);

    if (!IsRunning())
    {
        return;
    }

    m_stop = true;
    m_threads.clear();
}

const ThreadsInfo &TestRunner::GetThreadsInfo() const
{
    return m_threadsInfo;
}

void TestRunner::TerminateThreads()
{
    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        if (m_threads[i] != NULL)
        {
            ::TerminateThread(m_threads[i], 1);
            m_threads[i] = NULL;
        }
    }
}
