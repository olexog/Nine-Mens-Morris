#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NetHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotConnectionStatus(int status);
    void slotEnd(int res);

private:
    Ui::MainWindow *ui;
    NetHandler* m_pNetHandler;

private slots:
    void on_actionStop_triggered();
    void on_actionConnect_triggered();
};

#endif // MAINWINDOW_H
