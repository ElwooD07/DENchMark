#pragma once
#include "TestRunnerBase.h"
#include <Windows.h>

class TestRunnerWindows: public TestRunnerBase
{
public:
    TestRunnerWindows();
    ~TestRunnerWindows();
    virtual void Go(const TestParameters& parameters) override;
    virtual bool IsRunning() const override;
    virtual void Stop() override;

private:
    void TerminateThreads();
    void DeleteThreadObjects();

private:
    std::vector<HANDLE> m_threads;
};
