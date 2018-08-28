#pragma once
#include <stdint.h>
#include <vector>

enum TestMethod
{
    TestMethodUnknown = 0,
    TestMethodCPU,
    TestMethodMemory,
    TestMethodCPUandMemory
};

using Complexity = uint64_t;

struct TestParameters
{
    size_t numberOfThreads = 0;
    TestMethod method = TestMethodUnknown;
    Complexity baseComplexity = 0;
    bool burnMode = false;
};

using Progress = float; // from 0.0 to 1.0

struct ThreadInfo
{
    ThreadInfo(volatile bool& stop_, int64_t complexity_)
        : stop(stop_)
        , complexity(complexity_)
    {  }

    ThreadInfo& operator=(const ThreadInfo& right)
    {
        stop = right.stop;
        complexity = right.complexity;
        start = right.start;
        finish = right.finish;
        return *this;
    }

    volatile bool& stop;
    Complexity complexity;

    int64_t start = 0;
    int64_t finish = 0;

    Progress progress = 0.0;
};

using TestResult = uint64_t;
using TestResults = std::vector<uint64_t>;
using TheScore = uint64_t;
