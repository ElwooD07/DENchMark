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
    , m_burnEnabled(false)
{
    m_observer = new TestObserver(this, &m_testRunner);
    connect(m_observer, &QThread::finished, this, &MainWindow::OnTestFinished);

    m_ui->setupUi(this);
    m_ui->lblProgramName->setText(tr("%1 v%2.%3").arg(PROGRAM_NAME).arg(VER_MAJOR).arg(VER_MINOR));
    m_ui->lblThreads->setText(tr("Threads: %1").arg(m_processorsCount));
    m_ui->chkBurn->setChecked(m_burnEnabled);
}

void MainWindow::on_btnGo_clicked()
{
    try
    {
        m_testRunner.Go(m_processorsCount, DefineTestMethod(), utils::CalculateComplexity(m_burnEnabled));
        SetControlsEnabled(false);
        m_observer->start();
    }
    catch(const std::exception& ex)
    {
        QMessageBox::warning(this, tr("Start test"), tr("Unable to start the test: %1").arg(ex.what()));
    }
}

void MainWindow::on_chkBurn_stateChanged(Qt::CheckState state)
{
    m_burnEnabled = (state == Qt::Checked);
}

void MainWindow::OnTestFinished()
{
    Results results = m_observer->GetResults();
    m_ui->listWidget->clear();

    Results::value_type average = 0;
    Results::value_type max = 0;
    for (size_t i = 0; i < results.size(); ++i)
    {
        m_ui->listWidget->addItem(tr("Thread %1: %2").arg(i).arg(results.at(i)));
        average += results.at(i);
        if (max < results.at(i))
        {
            max = results.at(i);
        }
    }
    average /= results.size();

    m_ui->lblTimeElapsed->setText(QString::number(max));
    m_ui->lblScore->setText(QString::number(3000000 / average));

    SetControlsEnabled(true);
}

void MainWindow::SetControlsEnabled(bool enabled)
{
    m_ui->btnGo->setEnabled(enabled);
    m_ui->chkBurn->setEnabled(enabled);
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
