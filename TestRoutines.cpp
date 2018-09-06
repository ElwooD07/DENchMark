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

    try
    {
        info.startTimestamp = ::GetTickCount64();

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

        info.finishTimestamp = ::GetTickCount64();
        return 0;

    } catch (const std::exception& ex)
    {
        info.finishTimestamp = ::GetTickCount64();
        info.errorText = ex.what();
    }
    return 1;
}

DWORD ThreadProcMemory(PVOID parameters)
{ // Implements Erythrophene Sieve
    ThreadInfo& info = GetThreadInfo(parameters);

    try {
        std::vector<size_t> grid(info.complexity + 1);
        info.startTimestamp = ::GetTickCount64();

        size_t primes = 0;
        const size_t gridSize = grid.size();
        for (size_t i = 0; i < gridSize && !info.stop; ++i)
        {
            grid[i] = i;
        }

        for (size_t p = 2; p < gridSize && !info.stop; ++p)
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

        info.finishTimestamp = ::GetTickCount64();
        return 0;
    } catch (const std::exception& ex)
    {
        info.finishTimestamp = ::GetTickCount64();
        info.errorText = ex.what();
    }
    return 1;
}

DWORD ThreadProcCPUandMemory(PVOID parameters)
{
    GetThreadInfo(parameters).finishTimestamp = 100500;
    // TODO
    return 0;
}
