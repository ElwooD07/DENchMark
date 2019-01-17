#pragma once
#include "TestRunnerBase.h"
#include "TestStateKeeper.h"
#include <QThread>
#include <vector>

/*
 * Works with single test run. To get results of whole test, see the work of TestStateKeeper.
*/

class TestObserver: public QThread
{
public:
    TestObserver(QObject* parent, TestRunnerBase* runner);
    SingleTestResult GetSingleTestResult() const;

protected:
    virtual void run() override;

private:
    TestRunnerBase* m_runner;
};
