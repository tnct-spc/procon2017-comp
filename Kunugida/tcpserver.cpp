#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 9999)){
        qDebug() << "Server can not started.";
    }else{
        qDebug() << "Server started.";
    }
}

void TcpServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Welcome");
    socket->flush();
    socket->waitForBytesWritten(3000);
}
