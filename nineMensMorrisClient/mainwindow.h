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
    void connectToServer(QString addr);
    void disconnectFromServer();

private slots:
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();
    void on_pushButtonStartNewGame_clicked();
};

#endif // MAINWINDOW_H
