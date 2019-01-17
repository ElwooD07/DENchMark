#include "stdafx.h"
#include "TestRunnerBase.h"

TestRunnerBase::TestRunnerBase()
    : m_stop(false)
{ }

const ThreadsInfo&TestRunnerBase::GetThreadsInfo() const
{
    return m_threadsInfo;
}
