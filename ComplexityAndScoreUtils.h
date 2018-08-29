#pragma once
#include <stdint.h>
#include "TestInfo.h"

namespace utils
{
    Complexity CalculateComplexity();
    double GetTestMethodComplexityRatio(TestMethod method);
    TheScore CalculateTheScorePerThread(const StagesAverageResults &averageResults);
    TheScore CalculateTheScoreMultithreaded(const StagesAverageResults& averageResults, size_t numberOfThreads);
}
