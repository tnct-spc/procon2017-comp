#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject * parent = 0);

public slots:
    void newConnection();

private:
    QTcpServer *server;
};

#endif // TCPSERVER_H
