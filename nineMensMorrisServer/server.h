#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include "game.h"

#include <QFile>
#include <QTextStream>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();

    void StartServer();

protected:
    void SendState();
    void ParsePkg(int pl, const QByteArray& pkg);
    void CheckEnd();
    void StartGame();

signals:

public slots:
    void slotIncomingConn();
    void slotReadyRead1();
    void slotReadyRead2();
    void slotDisconnected();

    void slotSocket1Disconnected();
    void slotSocket2Disconnected();

protected:
    QTcpServer* m_pServSocket;
    QTcpSocket* m_pSocket1;
    QTcpSocket* m_pSocket2;

    unsigned char m_State[9];
    int m_CPlayer;
    int m_End;
    Game game;
    bool player1WaitingForStart;
    bool player2WaitingForStart;
    const int HEADER_LENGTH = 4;
    const int TABLE_SIZE = 24;

};

#endif // SERVER_H
