#include "stdafx.h"
#include "TestObserver.h"

TestObserver::TestObserver(QObject *parent, TestRunner* runner)
    : QThread(parent)
    , m_runner(runner)
{
    assert(m_runner);
}

Results TestObserver::GetResults() const
{
    assert(!m_runner->IsRunning());

    Results results;
    for (const ThreadInfo& threadInfo : m_runner->GetThreadsInfo())
    {
        results.push_back(threadInfo.finish - threadInfo.start);
    }
    return results;
}

void TestObserver::run()
{
    while(m_runner->IsRunning())
    {
        this->msleep(100);
    }
}
