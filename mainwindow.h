#pragma once
#include <memory>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "testrunner.h"
#include "testobserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);

private slots:
    void on_btnGo_clicked();

    void OnTestFinished();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    const size_t m_processorsCount;
    TestRunner m_testRunner;
    TestObserver* m_observer;
};
