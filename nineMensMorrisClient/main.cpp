#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QString serverName = "";
    if (argc > 0)
    {
        serverName = argv[0];
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
