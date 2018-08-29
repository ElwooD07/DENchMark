#pragma once
#include "TestInfo.h"

using TimesElapsed = std::vector<TimeElapsed>;

class TestObserver;
class TestStateKeeper
{
public:
    TestStateKeeper();
    void TestStarted(TestStage stages, bool burnIt);
    TestStage CurrentStage() const;
    TestStage GetStagesCount() const;
    void SubmitSingleTestResult(const SingleTestResult& result);
    StagesAverageResults GetAverageResults() const;
    TimesElapsed GetTimesElapsed() const;

private:
    TestStage m_stagesTotal;
    TestStage m_currentStage;
    std::vector<SingleTestResult> m_allResults;
};

