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

void TestStateKeeper::SubmitSingleTestResult(const SingleTestResult& result)
{
    m_allResults.push_back(result);
    ++m_currentStage;
}

StagesAverageResults TestStateKeeper::GetAverageResults() const
{
    StagesAverageResults averages;
    for (SingleTestResult stageResults : m_allResults)
    {
        StageAverageResult average = 0;
        for (const auto& stageResult : stageResults)
        {
            if (stageResult.error.empty())
            {
                average += stageResult.duration;
            }
            else
            {
                return {};
            }
        }
        averages.push_back(average / stageResults.size());
    }
    return averages;
}

TimesElapsed TestStateKeeper::GetTimesElapsed() const
{
    TimesElapsed timesElapsed;
    for (SingleTestResult testResult : m_allResults) // The entities of test results and time elapsed are similar for now.
    {
        TimeElapsed testTimeElapsed = 0;
        for (const auto& threadResult : testResult)
        {
            if (testTimeElapsed < static_cast<TimeElapsed>(threadResult.duration))
            {
                testTimeElapsed = static_cast<TimeElapsed>(threadResult.duration);
            }
        }
        timesElapsed.push_back(testTimeElapsed);
    }
    return timesElapsed;
}
