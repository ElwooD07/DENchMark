#pragma once
#include "TestRunner.h"
#include "TestStateKeeper.h"
#include <QThread>
#include <vector>

class TestObserver: public QThread
{
public:
    TestObserver(QObject* parent, TestRunner* runner);
    TestResults GetResults() const;

protected:
    virtual void run() override;

private:
    TestRunner *m_runner;
};
