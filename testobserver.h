#pragma once
#include "testrunner.h"
#include <QThread>
#include <vector>

using Results = std::vector<uint64_t>;

class TestObserver: public QThread
{
public:
    TestObserver(QObject* parent, TestRunner* runner);
    Results GetResults() const;

protected:
    virtual void run() override;

private:
    TestRunner *m_runner;
};
