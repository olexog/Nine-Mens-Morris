#include "nethandler.h"

#include <QList>

NetHandler::NetHandler(QObject *parent) :
    QObject(parent)
{
    m_pSocket = NULL;
}

NetHandler::~NetHandler()
{
    disconnect(this, SLOT(slotDisconnected()));

    if(m_pSocket) delete m_pSocket;
}

// Kapcsolodas a szerverhez.
void NetHandler::Connect(QString addr)
{
    // Biztonsagi ellenorzes. Ha letezik mar a socket, akkor
    // nem szabadna hogy ez a fuggveny meghivodjon.
    if (m_pSocket) return;

    // Letrehozzuk a kliens socket-et.
    m_pSocket = new QTcpSocket();

    // Jelezzuk a felhasznalo fele, hogy eppen kapcsolodni probalunk.
    emit signalConnectionStatus(Connecting);

    // Bekotjuk a kapcsolat bontasanak erzekeleset. Amikor a masik oldal lezarja
    // a kapcsolatot, akkor a kliens socket egy disconnected() szignalt general.
    connect(m_pSocket, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));

    // Bekotjuk a bejovo csomagok erzekeleset. A kovetkezo sorban levo kapcsolo
    // bekapcsolasaval elerhetjuk, hogy amikor csomag erkezik a kliens socketen
    // keresztul, akkor azt egy readyRead() szignallal jelezze. Igy nem kell
    // hogy folyamatosan varakozzunk az erkezo csomagokra.
    connect(m_pSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));


    // A connect() fuggveny meghivasaval kapcsolodunk a szerverhez.
    m_pSocket->connectToHost(addr, 3490);
    if (!m_pSocket->waitForConnected(5000)) {
        delete m_pSocket;
        m_pSocket = NULL;

        emit signalConnectionStatus(Error);
        return;
    }

    // Jelezzuk a kapcsolat allapotanak valtozasat, vagyis hogy sikerult.
    emit signalConnectionStatus(Connected);

}

// A kapcsolat lezarasanak erzekelese.
// Ha lezartak a kapcsolatot, akkor a szignal hatasara ez a fuggveny hivodik
// meg.
void NetHandler::slotDisconnected()
{
    // Kikotjuk a socket lezarasanak erzekeleset, mert kulonben a sajatunkat is
    // erzekelnenk.
    disconnect(this, SLOT(slotDisconnected()));

    // Ha letezik meg kliens socket akkor lezarjuk.
    if (m_pSocket) {
        m_pSocket->deleteLater();
        m_pSocket = NULL;
    }

    // Jelezzuk a kapcsolat allapotanak valtozasat.
    emit signalConnectionStatus(Disconnected);

}

// Csomag erkezesenek lekezelese.
// Amikor egy csomag erkezik, akkor a szignal hatasara ez a slot hivodik meg.
void NetHandler::slotReadyRead()
{
    QByteArray buf = m_pSocket->readAll().last(HEADER_LENGTH + TABLE_SIZE);

    QString filename = "../log.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << "CLIENT:reading ";
        for (int i = 0; i < HEADER_LENGTH + TABLE_SIZE; ++i)
        {
            stream << (int)buf[i];
        }
        stream << "\n";
    }

    Game::GameState gameState = static_cast<Game::GameState>(buf[0]);
    Game::ManColor manColor = static_cast<Game::ManColor>(buf[1]);
    int whiteMenToBePlaced = (int)buf[2];
    int blackMenToBePlaced = (int)buf[3];
    QList<Game::ManColor> table = QList<Game::ManColor>(TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        table[i] = (Game::ManColor)buf[i + HEADER_LENGTH];
    }

    Game receivedSituation = Game(table, whiteMenToBePlaced, blackMenToBePlaced, manColor, gameState);

    emit signalStateReceived(receivedSituation);

    //emit signalEnd(end);
}

// A lepes elkuldese a szervernek.
void NetHandler::slotSendNewState(Game newSituation)
{
    // Biztonsagi ellenorzes. Ha nem letezne a kliens socket valamilyen
    // okbol, akkor visszater a fuggveny.
    if (!m_pSocket) return;

    // Osszeallitjuk a protokoll szerinti uzenetet.
    QByteArray buf(HEADER_LENGTH + TABLE_SIZE,0);

    buf[0] = (quint8)newSituation.getState();
    buf[1] = (quint8)newSituation.getManColor();
    buf[2] = (quint8)newSituation.getWhiteMenToBePlaced();
    buf[3] = (quint8)newSituation.getBlackMenToBePlaced();

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        buf[i + HEADER_LENGTH] = (quint8)newSituation.getColorAt(i);
    }

    // A kliens socketen keresztul elkuldjuk a masik programnak.
    m_pSocket->write(buf);

}

