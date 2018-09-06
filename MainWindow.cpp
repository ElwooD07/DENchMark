#include "stdafx.h"
#include "MainWindow.h"
#include "ComplexityAndScoreUtils.h"

namespace
{
    size_t GetProcessorsCount()
    {
        ::SYSTEM_INFO sysInfo = { };
        ::GetSystemInfo(&sysInfo);
        return sysInfo.dwNumberOfProcessors;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_processorsCount(GetProcessorsCount())
    , m_observer(nullptr)
{
    m_observer = new TestObserver(this, &m_testRunner);
    connect(m_observer, &QThread::finished, this, &MainWindow::OnTestFinished);

    m_ui->setupUi(this);
    m_ui->lblProgramName->setText(tr("%1 v%2.%3").arg(PROGRAM_NAME).arg(VER_MAJOR).arg(VER_MINOR));
    m_ui->lblThreads->setText(tr("Threads: %1").arg(m_processorsCount));
}

void MainWindow::on_btnGo_clicked()
{
    m_ui->listWidget->clear();
    m_ui->progressBar->setValue(0);
    try
    {
        m_ui->lblScore->setText("0");
        m_ui->lblPerThread->setText("0");
        m_stateKeeper.TestStarted(DEFAULT_STAGES_COUNT, m_ui->chkBurn->isChecked());
        RunSingleTest();
    }
    catch(const std::exception& ex)
    {
        QMessageBox::warning(this, tr("Start test"), tr("Unable to start the test: %1").arg(ex.what()));
    }
}

void MainWindow::OnTestFinished()
{
    SingleTestResult result = m_observer->GetSingleTestResult();

    m_ui->listWidget->addItem(tr("Stage %1/%2").arg(m_stateKeeper.CurrentStage() + 1).arg(m_stateKeeper.GetStagesCount()));
    for (size_t i = 0; i < result.size(); ++i)
    {
        double threadDuration = static_cast<double>(result.at(i).duration) / 1000;
        if (result.at(i).error.empty())
        {
            m_ui->listWidget->addItem(QString("  ") + tr("Thread %1: %2").arg(i).arg(threadDuration));
        }
        else
        {
            m_ui->listWidget->addItem(QString("  ") + tr("Thread %1: error \"%2\"").arg(i).arg(result.at(i).error.c_str()));
        }
    }
    m_stateKeeper.SubmitSingleTestResult(m_observer->GetSingleTestResult());

    UpdateTimeElapsed();
    UpdateTheScore();

    if (m_stateKeeper.CurrentStage() < m_stateKeeper.GetStagesCount())
    {
        int progress = static_cast<int>((static_cast<double>(m_stateKeeper.CurrentStage()) / m_stateKeeper.GetStagesCount()) * 100);
        m_ui->progressBar->setValue(progress);
        m_ui->progressBar->update();
        RunSingleTest();
    }
    else
    {
        m_ui->progressBar->setValue(m_ui->progressBar->maximum());
        SetControlsEnabled(true);
    }
}

void MainWindow::SetControlsEnabled(bool enabled)
{
    m_ui->btnGo->setEnabled(enabled);
    m_ui->chkBurn->setEnabled(enabled);
    m_ui->optCPU->setEnabled(enabled);
    m_ui->optMemory->setEnabled(enabled);
}

TestMethod MainWindow::DefineTestMethod() const
{
    TestMethod method = TestMethodUnknown;
    if (m_ui->optCPU->isChecked())
    {
        method = TestMethodCPU;
    }
    if (m_ui->optMemory->isChecked())
    {
        method = TestMethodMemory;
    }
    else if (m_ui->optCPUandMemory->isChecked())
    {
        method = TestMethodCPUandMemory;
    }
    return method;
}

void MainWindow::RunSingleTest()
{
    m_testRunner.Go(TestParameters { m_processorsCount, DefineTestMethod(), utils::CalculateComplexity(), m_ui->chkBurn->isChecked() } );
    SetControlsEnabled(false);
    m_observer->start();
}

void MainWindow::UpdateTimeElapsed()
{
    TimesElapsed times = m_stateKeeper.GetTimesElapsed();
    TimeElapsed timeElapsedTotal = std::accumulate(times.begin(), times.end(), 0);
    m_ui->lblTimeElapsed->setText(tr("%1 seconds").arg(static_cast<float>(timeElapsedTotal) / 1000));
}

void MainWindow::UpdateTheScore()
{
    auto averageResults = m_stateKeeper.GetAverageResults();
    m_ui->lblScore->setText(QString::number(utils::CalculateTheScoreMultithreaded(averageResults, m_processorsCount)));
    m_ui->lblPerThread->setText(QString::number(utils::CalculateTheScorePerThread(averageResults)));
}
