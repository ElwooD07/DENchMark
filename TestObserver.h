#pragma once
#include "TestRunner.h"
#include "TestStateKeeper.h"
#include <QThread>
#include <vector>

/*
 * Works with single test run. To get results of whole test, see the work of TestStateKeeper.
*/

class TestObserver: public QThread
{
public:
    TestObserver(QObject* parent, TestRunner* runner);
    SingleTestResult GetSingleTestResult() const;

protected:
    virtual void run() override;

private:
    TestRunner *m_runner;
};
