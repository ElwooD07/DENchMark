#pragma once
#include <stdint.h>
#include "TestInfo.h"

namespace utils
{
    Complexity CalculateComplexity(bool burn);
    double GetTestMethodComplexityRatio(TestMethod method);
}
