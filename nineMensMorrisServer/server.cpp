#include "server.h"

Server::Server(QObject *parent) :
        QObject(parent)
{
    m_pServSocket = NULL;
    m_pSocket1 = NULL;
    m_pSocket2 = NULL;

    m_CPlayer = 0;
    m_End = 0;
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
    }
    else if (m_pSocket2 == NULL) {
        m_pSocket2 = pSocket;
        connect(m_pSocket2, SIGNAL(readyRead()),
                this, SLOT(slotReadyRead2()));
    }

    // játék kezdése
    if ((m_pSocket1 != NULL) && (m_pSocket2 != NULL))
    {
        Init();
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
    disconnect(this, SLOT(slotDisconnected()));

    // Ha letezik meg kliens socket akkor lezarjuk.
    if (m_pSocket1) {
        m_pSocket1->deleteLater();
        m_pSocket1 = NULL;
    }

    if (m_pSocket2) {
        m_pSocket2->deleteLater();
        m_pSocket2 = NULL;
    }
}

// Csomag erkezesenek lekezelese.
void Server::slotReadyRead1()
{
    QByteArray buf = m_pSocket1->read(28);
    if (buf.length() == 28)
        ParsePkg(1, buf);
}

// Csomag erkezesenek lekezelese.
void Server::slotReadyRead2()
{
    QByteArray buf = m_pSocket2->read(28);
    if (buf.length() == 28)
        ParsePkg(2, buf);
}

void Server::Init()
{
    srand(time(NULL));
    m_CPlayer = rand() % 2 + 1;
    m_End = 0;
    for (int i = 0; i < 9; ++i)
        m_State[i] = 0;
    SendState();
}

void Server::SendState()
{
    QByteArray buf(29,0);
    buf[0] = (quint8)this->game.gameState;
    buf[1] = 1; // black
    buf[2] = (quint8)this->game.whiteMenToBePlaced;
    buf[3] = (quint8)this->game.blackMenToBePlaced;
    buf[4] = (quint8)true;
    for (int i = 0; i < 24; ++i) {
        buf[i + 5] = this->game.gameTable[i];
    }

    QByteArray buf2(29,0);
    buf2[0] = (quint8)this->game.gameState;
    buf2[1] = 2; // white
    buf2[2] = (quint8)this->game.whiteMenToBePlaced;
    buf2[3] = (quint8)this->game.blackMenToBePlaced;
    buf2[4] = (quint8)true;
    for (int i = 0; i < 24; ++i) {
        buf2[i + 5] = this->game.gameTable[i];
    }

    m_pSocket1->write(buf);

    QString filename = "/home/olexo/Desktop/log.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << "SERVER:writing to socket 1: buf:";
        for (int i = 0; i < 29; ++i)
        {
            stream << (int)buf[i];
        }
        stream << "\n";
    }
    file.close();

    m_pSocket2->write(buf2);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << "SERVER:writing to socket 2: buf:";
        for (int i = 0; i < 29; ++i)
        {
            stream << (int)buf2[i];
        }
        stream << "\n";
    }
    file.close();
}

void Server::ParsePkg(int pl, const QByteArray& pkg)
{
    //if (pl != m_CPlayer) return;

    this->game.gameState = static_cast<Game::GameState>(pkg[0]);

    this->game.whiteMenToBePlaced = (int)pkg[2];
    this->game.blackMenToBePlaced = (int)pkg[3];

    for(int i = 0; i < 24; i++)
    {
        this->game.gameTable[i] = pkg[4 + i];
    }

    //CheckEnd();
    SendState();

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

// A jatek vegenek ellenorzese.
void Server::CheckEnd()
{
    unsigned char tmp;

    // Ellenorizzuk, hogy osszejott-e valamelyik nyero kombinacio.
    // Az atlok ellenorzese.
    tmp = m_State[0];
    if((tmp > 0) && (m_State[4] == tmp) && (m_State[8] == tmp))
    {
        m_End = tmp;
        return;
    }
    tmp = m_State[2];
    if((tmp > 0) && (m_State[4] == tmp) && (m_State[6] == tmp))
    {
        m_End = tmp;
        return;
    }
    // Vizszintes es fuggoleges kombinaciok ellenorzese.
    for(int i = 0; i < 3; i++)
    {
        tmp = m_State[i * 3];
        if((tmp > 0) && (m_State[i * 3 + 1] == tmp) && (m_State[i * 3 + 2] == tmp))
        {
            m_End = tmp;
            return;
        }
        tmp = m_State[i];
        if((tmp > 0) && (m_State[3 + i] == tmp) && (m_State[6 + i] == tmp))
        {
            m_End = tmp;
            return;
        }
    }

    // Ha egyik nyero kombinacio sem jott ki, akkor ellenorizzuk van-e meg ures
    // pozicio, mert kulonben dontetlen.
    for(int i = 0; i < 9; i++)
    {
        if(m_State[i] == 0)
        {
            m_End = 0;
            return;
        }
    }

    m_End = 3;
    return;
}
