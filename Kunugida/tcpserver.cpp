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

    connect(this, SIGNAL(request()), this, SLOT(send()));
}

void TcpServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Welcome");
    socket->flush();
    socket->waitForBytesWritten(3000);
}

void TcpServer::getRequest()
{
    emit request();
}

void TcpServer::send()
{
    NeoPolygonIO::exportPolygon(field, "../../procon2017-comp/field.csv");
}

void TcpServer::setField(procon::NeoField input_field)
{
    field = input_field;
}
