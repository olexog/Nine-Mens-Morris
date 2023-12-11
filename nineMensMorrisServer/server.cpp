#include "server.h"
#include "game.h"

Server::Server(QObject *parent) :
        QObject(parent)
{
    m_pServSocket = NULL;
    m_pSocket1 = NULL;
    m_pSocket2 = NULL;

    player1WaitingForStart = false;
    player2WaitingForStart = false;
}

Server::~Server()
{
    delete m_pServSocket;
    delete m_pSocket1;
    delete m_pSocket2;
}

void Server::StartServer()
{
    if (m_pServSocket) return;
    // Letrehozzuk a szerver socket-et.

    m_pServSocket = new QTcpServer();
    if (!m_pServSocket->listen(
                QHostAddress::Any, 3490)) {
        delete m_pServSocket;
        m_pServSocket = NULL;
        return;
    }

    // Aszinkron modon, szignal-al figyeljuk a kapcsolodasokat.
    connect(m_pServSocket, SIGNAL(newConnection()),
            this, SLOT(slotIncomingConn()));

}

// A bejovo kapcsolatok fogadasa.
void Server::slotIncomingConn()
{
    QTcpSocket* pSocket =
            m_pServSocket->nextPendingConnection();

    if ((m_pSocket1 != NULL) && (m_pSocket2 != NULL)) {
        delete pSocket;
        return;
    }

    connect(pSocket, SIGNAL(disconnected()), this,
            SLOT(slotDisconnected()));

    if (m_pSocket1 == NULL) {
        m_pSocket1 = pSocket;
        connect(m_pSocket1, SIGNAL(readyRead()),
                this, SLOT(slotReadyRead1()));
        connect(m_pSocket1, SIGNAL(disconnected()),
                this, SLOT(slotSocket1Disconnected()));
        SendState();

    }
    else if (m_pSocket2 == NULL) {
        m_pSocket2 = pSocket;
        connect(m_pSocket2, SIGNAL(readyRead()),
                this, SLOT(slotReadyRead2()));
        connect(m_pSocket2, SIGNAL(disconnected()),
                this, SLOT(slotSocket2Disconnected()));
        SendState();

    }

    // játék kezdése
    if ((m_pSocket1 != NULL) && (m_pSocket2 != NULL))
    {
        this->StartGame();
    }
}

void Server::StartGame()
{
    this->game = Game();
    this->SendState();
}

// A kapcsolat lezarasanak erzekelese.
void Server::slotDisconnected()
{
    //disconnect(this, SLOT(slotDisconnected()));

    // Ha letezik meg kliens socket akkor lezarjuk.
    /*if (!m_pSocket1) {
        m_pSocket1->deleteLater();
        m_pSocket1 = NULL;
    }

    if (!m_pSocket2) {
        m_pSocket2->deleteLater();
        m_pSocket2 = NULL;
    }*/
}

void Server::slotSocket1Disconnected()
{
    if (m_pSocket1) {
        m_pSocket1->deleteLater();
        m_pSocket1 = NULL;
    }
    player1WaitingForStart = false;
    SendState();
}

void Server::slotSocket2Disconnected()
{
    if (m_pSocket2) {
        m_pSocket2->deleteLater();
        m_pSocket2 = NULL;
    }
    player2WaitingForStart = false;
    SendState();
}

// Csomag erkezesenek lekezelese.
void Server::slotReadyRead1()
{
    QByteArray buf = m_pSocket1->read(HEADER_LENGTH + TABLE_SIZE);
    if (buf.length() == HEADER_LENGTH + TABLE_SIZE)
        ParsePkg(1, buf);
}

// Csomag erkezesenek lekezelese.
void Server::slotReadyRead2()
{
    QByteArray buf = m_pSocket2->read(HEADER_LENGTH + TABLE_SIZE);
    if (buf.length() == HEADER_LENGTH + TABLE_SIZE)
        ParsePkg(2, buf);
}

void Server::SendState()
{
    QString filename = "../log.txt";
    QFile file(filename);

    QByteArray buf(HEADER_LENGTH + TABLE_SIZE,0);
    buf[0] = (quint8)this->game.gameState;
    buf[1] = (quint8)Game::Black;
    buf[2] = (quint8)this->game.whiteMenToBePlaced;
    buf[3] = (quint8)this->game.blackMenToBePlaced;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        buf[i + HEADER_LENGTH] = this->game.gameTable[i];
    }

    if (m_pSocket1)
    {
        m_pSocket1->write(buf);

        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&file);
            stream << "SERVER:writing to socket 1: buf:";
            for (int i = 0; i < HEADER_LENGTH + TABLE_SIZE; ++i)
            {
                stream << (int)buf[i];
            }
            stream << "\n";
        }
        file.close();
    }

    buf[1] = (quint8)Game::White;

    if (m_pSocket2)
    {
        m_pSocket2->write(buf);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream(&file);
            stream << "SERVER:writing to socket 2: buf:";
            for (int i = 0; i < HEADER_LENGTH + TABLE_SIZE; ++i)
            {
                stream << (int)buf[i];
            }
            stream << "\n";
        }
        file.close();
    }
}

void Server::ParsePkg(int pl, const QByteArray& pkg)
{
    //if (pl != m_CPlayer) return;

    this->game.gameState = static_cast<Game::GameState>(pkg[0]);

    this->game.whiteMenToBePlaced = (int)pkg[2];
    this->game.blackMenToBePlaced = (int)pkg[3];

    for(int i = 0; i < TABLE_SIZE; i++)
    {
        this->game.gameTable[i] = pkg[HEADER_LENGTH + i];
    }

    if (pl == 1)
        player1WaitingForStart = this->game.gameState == Game::ReadyForNewGame;
    else if (pl == 2)
        player2WaitingForStart = this->game.gameState == Game::ReadyForNewGame;

    if (player1WaitingForStart && player2WaitingForStart)
    {
        StartGame();
    }
    else
    {
        SendState();
    }

    /*int i = y*3+x;
    if (m_State[i] == 0) {
        m_State[i] = pl;
        m_CPlayer = (m_CPlayer % 2) + 1;

        CheckEnd();
        SendState();

        if (m_End != 0) {
            m_CPlayer = 0;
            QTimer::singleShot(100, this,
                               SLOT(slotDisconnected()));
        }
    }*/

}
