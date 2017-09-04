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
    connect(this, SIGNAL(ready()), this, SLOT(receive()));
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
    QTcpSocket *client = server->nextPendingConnection();
    QDataStream out(client);
    out.setVersion(QDataStream::Qt_5_8);
    std::string file_path = "../../procon2017-comp/field.csv";
    NeoPolygonIO::exportPolygon(field, file_path);
    std::ifstream input(file_path);
    std::string line_buffer = "";
    while(std::getline(input, line_buffer)){
        QString f = f + QString::fromStdString(line_buffer);
    }
    qint16 blocksize = f.size();
    out << blocksize << f;
    emit ready();
}

void receive()
{

}

void TcpServer::setField(procon::NeoField input_field)
{
    field = input_field;
}
