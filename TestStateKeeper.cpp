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
        StageAverageResult average = std::accumulate(stageResults.begin(), stageResults.end(), 0);
        averages.push_back(average / stageResults.size());
    }
    return averages;
}

TimesElapsed TestStateKeeper::GetTimesElapsed() const
{
    TimesElapsed timesElapsed;
    for (SingleTestResult results : m_allResults) // The entities of test results and time elapsed are similar for now.
    {
        timesElapsed.push_back(*std::max_element(results.begin(), results.end()));
    }
    return timesElapsed;
}
