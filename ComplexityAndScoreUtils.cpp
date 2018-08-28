#include "stdafx.h"
#include "ComplexityAndScoreUtils.h"

Complexity utils::CalculateComplexity(bool burn)
{
    size_t complexity = 100000;
    if (burn)
    {
        complexity *= 4;
    }
    return complexity;
}

double utils::GetTestMethodComplexityRatio(TestMethod method)
{
    switch(method)
    {
    case TestMethodCPU:
        return 1;
    case TestMethodMemory:
        return 855;
    case TestMethodCPUandMemory:
        return 1; // TODO
    default:
        assert("Unknown test method");
        return 1;
    }
}
