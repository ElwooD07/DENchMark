#pragma once
#include <memory>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "TestRunner.h"
#include "TestObserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);

private slots:
    void on_btnGo_clicked();    
    void on_chkBurn_stateChanged(Qt::CheckState state);
    void OnTestFinished();

private:
    void SetControlsEnabled(bool enabled);
    TestMethod DefineTestMethod() const;

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    const size_t m_processorsCount;
    TestRunner m_testRunner;
    TestObserver* m_observer;
    bool m_burnEnabled;
};
