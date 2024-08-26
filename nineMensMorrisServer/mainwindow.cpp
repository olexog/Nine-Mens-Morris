#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pServer = new Server(this);
    m_pServer->StartServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}
