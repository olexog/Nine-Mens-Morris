#ifndef NETHANDLER_H
#define NETHANDLER_H

#include <QObject>
#include <QtNetwork>

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
    void signalState(bool cplayer, unsigned char* state);
    void signalEnd(int end);

public slots:
    void slotReadyRead();
    void slotDisconnected();

    void slotStep(int x, int y);

protected:
    QTcpSocket* m_pSocket;
};

#endif // NETHANDLER_H
