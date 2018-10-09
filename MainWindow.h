#pragma once
#include <memory>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "TestRunner.h"
#include "TestObserver.h"
#include "TestStateKeeper.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);

private slots:
    void on_btnGo_clicked();    
    void OnTestFinished();

private:
    void SetControlsEnabled(bool enabled);
    TestMethod DefineTestMethod() const;

    void RunSingleTest();
    void UpdateList();
    void UpdateTimeElapsed();
    void UpdateTheScore();
    void UpdateProgress();
    void ShowTheScore();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    const size_t m_processorsCount;
    TestRunner m_testRunner;
    TestObserver* m_observer;
    TestStateKeeper m_stateKeeper;
};
