#include "stdafx.h"
#include "TestRoutines.h"
#include "TestInfo.h"

namespace
{
    inline ThreadInfo& GetThreadInfo(PVOID parameters)
    {
        return *(reinterpret_cast<ThreadInfo*>(parameters));
    }
}

DWORD ThreadProcCPU(PVOID parameters)
{
    ThreadInfo& info = GetThreadInfo(parameters);
    info.start = ::GetTickCount64();

    size_t primesFound = 1; // 2 is already counted
    for (Complexity i = 2; i < info.complexity && !info.stop; ++i)
    {
        bool isPrime = false;
        for (Complexity j = 2; j < i && !info.stop; ++j)
        {
            if (i % j == 0)
            {
                break;
            }
        }

        if (isPrime)
        {
            ++primesFound;
        }
    }

    info.finish = ::GetTickCount64();
    return 0;
}

DWORD ThreadProcMemory(PVOID parameters)
{ // Implements Erythrophene Sieve
    ThreadInfo& info = GetThreadInfo(parameters);
    std::vector<size_t> grid(info.complexity + 1);
    info.start = ::GetTickCount64();

    size_t primes = 0;
    const size_t gridSize = grid.size();
    for (int i = 0; i < gridSize && !info.stop; i++)
    {
        grid[i] = i;
    }

    for (size_t p = 2; p < gridSize && !info.stop; p++)
    {
        if (grid[p] != 0)
        {
            ++primes;
            for (size_t j = p * p; j < gridSize; j += p)
            { // Mark not prime numbers
                grid[j] = 0;
            }
        }
    }

    info.finish = ::GetTickCount64();
    return 0;
}

DWORD ThreadProcCPUandMemory(PVOID parameters)
{
    GetThreadInfo(parameters).finish = 100500;
    // TODO
    return 0;
}
