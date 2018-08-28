#include "stdafx.h"
#include "ComplexityAndScoreUtils.h"

Complexity utils::CalculateComplexity()
{
    size_t complexity = 100000;
    // TODO: Adjust base complexity accordingly to system specs
    return complexity;
}

double utils::GetTestMethodComplexityRatio(TestMethod method)
{
    switch(method)
    {
    case TestMethodCPU:
        return 1;
    case TestMethodMemory:
        return 808;
    case TestMethodCPUandMemory:
        return 1; // TODO
    default:
        assert("Unknown test method");
        return 1;
    }
}

TheScore utils::CalculateTheScorePerThread(const TestResults& averageResults)
{
    TestResult average = 0;
    for (auto singleAverage : averageResults)
    {
       average += singleAverage;
    }
    average /= averageResults.size();

    return SCORE_STANDARD_DIVIDEND / average;
}

TheScore utils::CalculateTheScoreMultithreaded(const TestResults& averageResults, size_t numberOfThreads)
{
    TestResult total = 0;
    for (auto singleAverage : averageResults)
    {
       total += singleAverage;
    }
    return total;
}
