#include "stdafx.h"
#include "TestStateKeeper.h"
#include "TestObserver.h"

TestStateKeeper::TestStateKeeper()
    : m_stagesTotal(0)
    , m_currentStage(0)
{ }

void TestStateKeeper::TestStarted(TestStage stages, bool burnIt)
{
    m_currentStage = 0;
    m_allResults.clear();
    if (burnIt)
    {
        stages *= 4;
    }
    m_stagesTotal = stages;
}

TestStage TestStateKeeper::CurrentStage() const
{
    return m_currentStage;
}

TestStage TestStateKeeper::GetStagesCount() const
{
    return m_stagesTotal;
}

void TestStateKeeper::SubmitResults(TestObserver* observer)
{
    assert(observer && "You've forget something important");
    m_allResults.push_back(observer->GetResults());
    ++m_currentStage;
}

TestResults TestStateKeeper::GetAverageResults() const
{
    TestResults averages;
    for (TestResults results : m_allResults)
    {
        TestResult average = 0;
        for (size_t i = 0; i < results.size(); ++i)
        {
            average += results.at(i);
        }
        averages.push_back(average / results.size());
    }

    return averages;
}

TestResults TestStateKeeper::GetTimesElapsed() const
{
    TestResults timesElapsed;
    for (TestResults results : m_allResults)
    {
        TestResult max = 0;
        for (size_t i = 0; i < results.size(); ++i)
        {
            if (max < results.at(i))
            {
                max = results.at(i);
            }
        }
        timesElapsed.push_back(max);
    }

    return timesElapsed;
}
