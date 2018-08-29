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
using TimeElapsed = int64_t;

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
        startTimestamp = right.startTimestamp;
        finishTimestamp = right.finishTimestamp;
        return *this;
    }

    volatile bool& stop;
    Complexity complexity;

    int64_t startTimestamp = 0;
    int64_t finishTimestamp = 0;

    Progress progress = 0.0;
};

using SingleThreadResult = uint64_t;
using SingleTestResult = std::vector<SingleThreadResult>;

using StageAverageResult = SingleThreadResult;
using StagesAverageResults = std::vector<StageAverageResult>;

using TestStage = uint16_t;
using TheScore = uint64_t;
