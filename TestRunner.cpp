#include "stdafx.h"
#include "TestRunner.h"
#include "TestRoutines.h"
#include "ComplexityAndScoreUtils.h"

namespace
{
    inline LPTHREAD_START_ROUTINE SelectThreadProcForTestMethod(TestMethod method)
    {
        switch(method)
        {
        case TestMethodCPU:
            return &ThreadProcCPU;
        case TestMethodMemory:
            return &ThreadProcMemory;
        case TestMethodCPUandMemory:
            return &ThreadProcCPUandMemory;
        default:
            throw std::runtime_error("Unknown test method");
        }
    }
}

TestRunner::TestRunner()
    : m_stop(false)
{
    if (!::SetPriorityClass(::GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS))
    {
        throw std::runtime_error("Can't set process priority. Try to launch this application with Administrative privilegies");
    }
}

void TestRunner::Go(const TestParameters& parameters)
{
    std::unique_lock<std::mutex> lock(m_startStopMutex);

    assert(parameters.method != TestMethodUnknown && "Something goes wrong up on stack");
    if (IsRunning())
    {
        return;
    }

    if (!parameters.burnMode)
    {
        for (uint16_t i = 0; i < 350; ++i)
        {
            QApplication::processEvents();
            QThread::msleep(1);
        }
    }

    const Complexity complexity = static_cast<int64_t>(parameters.baseComplexity * utils::GetTestMethodComplexityRatio(parameters.method));

    m_stop = false;
    m_threads.assign(parameters.numberOfThreads, NULL);
    m_threadsInfo.assign(parameters.numberOfThreads, ThreadInfo(m_stop, complexity));

    LPTHREAD_START_ROUTINE threadProc = SelectThreadProcForTestMethod(parameters.method);
    for (size_t i = 0; i < parameters.numberOfThreads; ++i)
    {
        m_threads[i] = ::CreateThread(NULL, 0, threadProc, &(m_threadsInfo[i]), CREATE_SUSPENDED, NULL);
        if (m_threads[i] == NULL)
        {
            TerminateThreads();
            throw std::runtime_error("Unable to create thread");
        }
        if (!::SetThreadPriority(m_threads.at(i), THREAD_PRIORITY_TIME_CRITICAL))
        {
            throw std::runtime_error("Unable to adjust thread priority");
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

void TestRunner::TerminateThreads() // This is awful idea, I should think a something better
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
