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

TheScore utils::CalculateTheScorePerThread(const StagesAverageResults& averageResults)
{
    StageAverageResult average = std::accumulate(averageResults.begin(), averageResults.end(), 0);
    average /= averageResults.size();

    return average > 0 ? SCORE_STANDARD_DIVIDEND / average : 0;
}

TheScore utils::CalculateTheScoreMultithreaded(const StagesAverageResults& averageResults, size_t numberOfThreads)
{
    return CalculateTheScorePerThread(averageResults) * numberOfThreads;
}
