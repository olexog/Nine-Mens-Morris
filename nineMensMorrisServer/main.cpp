#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QString filename = "../log.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream << "SERVER:program started" << '\n';
    }

    QApplication a(argc, argv);
    MainWindow w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}
