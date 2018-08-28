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
    TestResults results = m_observer->GetResults();

    m_ui->listWidget->addItem(tr("Stage %1/%2").arg(m_stateKeeper.CurrentStage() + 1).arg(m_stateKeeper.GetStagesCount()));
    for (size_t i = 0; i < results.size(); ++i)
    {
        float resultFormated = static_cast<float>(results.at(i)) / 1000;
        m_ui->listWidget->addItem(QString("  ") + tr("Thread %1: %2").arg(i).arg(resultFormated));
    }
    m_stateKeeper.SubmitResults(m_observer);

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
    TestResults times = m_stateKeeper.GetTimesElapsed();
    TestResult timeElapsed = 0;
    for (auto time : times)
    {
        timeElapsed += time;
    }
    m_ui->lblTimeElapsed->setText(tr("%1 seconds").arg(static_cast<float>(timeElapsed) / 1000));
}

void MainWindow::UpdateTheScore()
{
    auto averageResults = m_stateKeeper.GetAverageResults();
    m_ui->lblScore->setText(QString::number(utils::CalculateTheScoreMultithreaded(averageResults, m_processorsCount)));
    m_ui->lblPerThread->setText(QString::number(utils::CalculateTheScorePerThread(averageResults)));
}
