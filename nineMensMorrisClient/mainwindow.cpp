#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nethandler.h"
#include "game.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pNetHandler = new NetHandler(this);

    connect(m_pNetHandler,
            SIGNAL(signalConnectionStatus(int)),
            this, SLOT(slotConnectionStatus(int)));
    connect(m_pNetHandler, SIGNAL(signalEnd(int)),
            this, SLOT(slotEnd(int)));

    connect(m_pNetHandler,
            SIGNAL(signalStateReceived(Game)),
            ui->nmmwidget,
            SLOT(slotStateReceived(Game)));
    connect(ui->nmmwidget,
            SIGNAL(signalSendNewState(Game)),
            m_pNetHandler,
            SLOT(slotSendNewState(Game)));

    connect(ui->nmmwidget, SIGNAL(signalUpdateLabels(QString,QString,QString)),
            this, SLOT(slotUpdateLabels(QString,QString,QString)));

    if (QCoreApplication::arguments().count() > 1)
    {
        QString arg = QCoreApplication::arguments().at(1);
        connectToServer(arg);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotUpdateLabels(QString colorInfo, QString direction, QString gameInfo)
{
    ui->labelColorInfo->setText(colorInfo);
    ui->labelDirections->setText(direction);
    ui->labelGameInfo->setText(gameInfo);
}

// A kapcsolat allapotanak kijelzese es a menuelemek allitasa.
// Attol fuggoen, hogy mi a halozati kapcsolat allapota kulonbozo szoveget irunk ki,
// illetve kulonbozo menupontokat engedelyezunk.
void MainWindow::slotConnectionStatus(int status)
{
    // A halozati kapcsolat allapotanak kiirasa a status bar-ra.
    switch(status)
    {
    case NetHandler::Error:
        statusBar()->showMessage("Error");
        break;
    case NetHandler::Connecting:
        statusBar()->showMessage("Connecting...");
        break;
    case NetHandler::Connected:
        statusBar()->showMessage("Connected");
        break;
    case NetHandler::Disconnected:
        statusBar()->showMessage("Disconnected");
        break;
    }
    if((status == NetHandler::Error) || (status == NetHandler::Disconnected))
    {
        ui->actionConnect->setEnabled(true);
        ui->actionStop->setEnabled(false);
    }
    else if((status == NetHandler::Connecting) || (status == NetHandler::Connected))
    {
        ui->actionConnect->setEnabled(false);
        ui->actionStop->setEnabled(true);
    }
}

// A jatek vege jelzes lekezelese.
void MainWindow::slotEnd(int res)
{
}

void MainWindow::connectToServer(QString addr)
{
    m_pNetHandler->Connect(addr);
    ui->nmmwidget->slotInit();
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(true);
}

void MainWindow::disconnectFromServer()
{
    m_pNetHandler->slotDisconnected();
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonDisconnect->setEnabled(false);
}

void MainWindow::on_pushButtonConnect_clicked()
{
    // Bekerjuk a masik gep cimet. Ha ez nem nulla hosszusagu, akkor kapcsolodunk hozza.
    bool ok;
    QString addr = QInputDialog::getText(this,
                                         "Server address", "Please enter the server host:",
                                         QLineEdit::Normal, "localhost", &ok);
    if(ok && !addr.isEmpty())
    {
        connectToServer(addr);
    }
}


void MainWindow::on_pushButtonDisconnect_clicked()
{
    disconnectFromServer();
}
