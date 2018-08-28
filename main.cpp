#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try
    {
        MainWindow w(nullptr);
        w.show();
        return a.exec();
    }
    catch(const std::exception& ex)
    {
        QMessageBox::warning(nullptr, "Fatal error", ex.what());
    }
    return 1;
}
