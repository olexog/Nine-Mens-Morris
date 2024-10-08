#ifndef NETHANDLER_H
#define NETHANDLER_H

#include <QObject>
#include <QtNetwork>
#include "game.h"

class NetHandler : public QObject
{
    Q_OBJECT
public:
    enum ConnStatus { Error, Connecting, Connected, Disconnected };
    enum Result { none = 0, winnerX = 1, winnerO = 2, drew = 3 };
public:
    explicit NetHandler(QObject *parent = 0);
    virtual ~NetHandler();
    void Connect(QString addr);

signals:
    void signalConnectionStatus(int status);
    void signalStateReceived(Game receivedSituation);
    void signalEnd(int end);

public slots:
    void slotReadyRead();
    void slotDisconnected();

    void slotSendNewState(Game newSituation);

protected:
    QTcpSocket* m_pSocket;
    const int HEADER_LENGTH = 4;
    const int TABLE_SIZE = 24;
};

#endif // NETHANDLER_H
