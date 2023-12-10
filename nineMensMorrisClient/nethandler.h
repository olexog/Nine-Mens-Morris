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
    void signalState(int state, int manColor, unsigned char* table);
    void signalEnd(int end);

public slots:
    void slotReadyRead();
    void slotDisconnected();

    void slotStep(Game newSituation);

protected:
    QTcpSocket* m_pSocket;
};

#endif // NETHANDLER_H
