#pragma once
#include <stdint.h>
#include "TestInfo.h"

namespace utils
{
    Complexity CalculateComplexity();
    double GetTestMethodComplexityRatio(TestMethod method);
    TheScore CalculateTheScorePerThread(const TestResults& averageResults);
    TheScore CalculateTheScoreMultithreaded(const TestResults& averageResults, size_t numberOfThreads);
}
