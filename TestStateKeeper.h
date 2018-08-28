#pragma once
#include "TestInfo.h"

using TestStage = uint16_t;

class TestObserver;
class TestStateKeeper
{
public:
    TestStateKeeper();
    void TestStarted(TestStage stages, bool burnIt);
    TestStage CurrentStage() const;
    TestStage GetStagesCount() const;
    void SubmitResults(TestObserver* observer);
    TestResults GetAverageResults() const;
    TestResults GetTimesElapsed() const;

private:
    TestStage m_stagesTotal;
    TestStage m_currentStage;
    std::vector<TestResults> m_allResults;
};

