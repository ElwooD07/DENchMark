#include "stdafx.h"
#include "TestObserver.h"

TestObserver::TestObserver(QObject *parent, TestRunnerBase* runner)
    : QThread(parent)
    , m_runner(runner)
{
    assert(m_runner);
}

SingleTestResult TestObserver::GetSingleTestResult() const
{
    assert(!m_runner->IsRunning());

    SingleTestResult results;
    std::transform(m_runner->GetThreadsInfo().begin(), m_runner->GetThreadsInfo().end(), std::back_inserter(results),
                   [](const ThreadInfo& threadInfo) {
                        return SingleThreadResult { threadInfo.finishTimestamp - threadInfo.startTimestamp, threadInfo.errorText } ;
                    });
    return results;
}

void TestObserver::run()
{
    while(m_runner->IsRunning())
    {
        this->msleep(100);
    }
}
