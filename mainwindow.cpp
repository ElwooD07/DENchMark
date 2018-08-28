#include "mainwindow.h"
#include <QMessageBox>
#include <Windows.h>

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
    m_ui->lblThreads->setText(tr("Threads: %1").arg(m_processorsCount));
}

void MainWindow::on_btnGo_clicked()
{
    try
    {
        TestMethod method = CPU;
        if (m_ui->optMemory->isChecked())
        {
            method = Memory;
        }
        else if (m_ui->optCPUandMemory->isChecked())
        {
            method = CPUandMemory;
        }
        size_t complexity = 100000;
        if (m_ui->chkBurn->isChecked())
        {
            complexity *= 4;
        }
        m_testRunner.Go(m_processorsCount, method, complexity);
        m_ui->btnGo->setEnabled(false);
        m_observer->start();
    }
    catch(const std::exception& ex)
    {
        QMessageBox::warning(this, tr("Start test"), tr("Unable to start the test: %1").arg(ex.what()));
    }
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
    m_ui->btnGo->setEnabled(true);
}
