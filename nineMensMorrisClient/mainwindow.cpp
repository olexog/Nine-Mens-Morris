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
            ui->tttwidget,
            SLOT(slotStateReceived(Game)));
    connect(ui->tttwidget,
            SIGNAL(signalSendNewState(Game)),
            m_pNetHandler,
            SLOT(slotSendNewState(Game)));

    if (QCoreApplication::arguments().count() > 1)
    {
        QString arg = QCoreApplication::arguments().at(1);
        m_pNetHandler->Connect(arg);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
    // Osszeallitjuk a megjelenitendo uzenetet a jelzes parametere alapjan.
    QString msg;
    switch(res)
    {
    case NetHandler::winnerX:
        msg = "The winner is X!";
        break;
    case NetHandler::winnerO:
        msg = "The winner is O!";
        break;
    case NetHandler::drew:
        msg = "Game End!";
        break;
    }
    QMessageBox::information(this, "Game end", msg);
}

void MainWindow::on_actionConnect_triggered()
{
    // Bekerjuk a masik gep cimet. Ha ez nem nulla hosszusagu, akkor kapcsolodunk hozza.
    bool ok;
    QString addr = QInputDialog::getText(this,
      "Server address", "Please enter the server host:",
      QLineEdit::Normal, "localhost", &ok);
    if(ok && !addr.isEmpty())
    {
        m_pNetHandler->Connect(addr);
        ui->tttwidget->slotInit();
    }
}

void MainWindow::on_actionStop_triggered()
{
    m_pNetHandler->slotDisconnected();
}
