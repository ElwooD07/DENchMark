#include "stdafx.h"
#include "TestRoutinesWindows.h"
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
{
#ifdef _DEBUG
    static const size_t s_iterationsCount = 3;
#else
    static const size_t s_iterationsCount = 30;
#endif

    ThreadInfo& info = GetThreadInfo(parameters);

    try
    {
        info.startTimestamp = ::GetTickCount64();

        for (size_t iteration = 0; iteration < s_iterationsCount && !info.stop; ++iteration)
        {
            std::vector<int32_t> vec1, vec2;
            vec1.resize(info.complexity);
            vec2.resize(info.complexity);

            for (size_t posForward = 0; posForward < vec1.size(); ++posForward)
            {
                auto tmp = vec1[posForward];
                size_t posBack = vec2.size() - posForward - 1;
                vec1[posForward] = vec2[posBack];
                vec2[posBack] = tmp;
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
{ // Implements Erythrophene Sieve
    ThreadInfo& info = GetThreadInfo(parameters);

    try {
        std::vector<uint8_t> grid(info.complexity + 1, 1);
        info.startTimestamp = ::GetTickCount64();

        size_t primes = 0;
        for (size_t p = 2; p < grid.size() && !info.stop; ++p)
        {
            if (grid.at(p) != 0)
            {
                ++primes;
                for (size_t j = 2 * p; j < grid.size(); j += p)
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
