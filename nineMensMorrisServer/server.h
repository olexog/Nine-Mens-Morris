#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();

    void StartServer();

protected:
    void Init();
    void SendState();
    void ParsePkg(int pl, const QByteArray& pkg);
    void CheckEnd();

signals:

public slots:
    void slotIncomingConn();
    void slotReadyRead1();
    void slotReadyRead2();
    void slotDisconnected();

protected:
    QTcpServer* m_pServSocket;
    QTcpSocket* m_pSocket1;
    QTcpSocket* m_pSocket2;

    unsigned char m_State[9];
    int m_CPlayer;
    int m_End;
};

#endif // SERVER_H
