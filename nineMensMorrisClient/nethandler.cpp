#include "nethandler.h"

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
    QByteArray buf = m_pSocket->read(24 + 2);
    m_pSocket->readAll();

    QString filename = "/home/olexo/Desktop/log.txt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << "CLIENT:reading ";
        for (int i = 0; i < 26; ++i)
        {
            stream << (int)buf[i];
        }
        stream << "\n";
    }

    int state = buf[0];
    int manColor = buf[1];

    unsigned char gameTable[24];

    for (int i = 0; i < 24; i++)
    {
        gameTable[i] = buf[i + 2];
    }

    emit signalState(state, manColor, gameTable);

    //emit signalEnd(end);
}

// A lepes elkuldese a szervernek.
void NetHandler::slotStep(Game newSituation)
{
    // Biztonsagi ellenorzes. Ha nem letezne a kliens socket valamilyen
    // okbol, akkor visszater a fuggveny.
    if (!m_pSocket) return;

    // Osszeallitjuk a protokoll szerinti uzenetet.
    QByteArray buf(26,0);

    buf[0] = (quint8)newSituation.gameState;
    buf[1] = (quint8)newSituation.manColor;

    for (int i = 0; i < 24; i++)
    {
        buf[i + 2] = (quint8)newSituation.table[i];
    }

    // A kliens socketen keresztul elkuldjuk a masik programnak.
    m_pSocket->write(buf);

}

