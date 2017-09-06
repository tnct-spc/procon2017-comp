#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 9999)){
        qDebug() << "Server can not started.";
    }else{
        qDebug() << "Server started.";
    }

    connect(this, SIGNAL(ready()), this, SLOT(receive()));
}

void TcpServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Welcome");
    socket->flush();
    socket->waitForBytesWritten(3000);
}

void TcpServer::getRequest(procon::NeoField input_field)
{
    field = input_field;
    TcpServer::send();
}

void TcpServer::send()
{
    QTcpSocket *client = server->nextPendingConnection();
    QDataStream out(client);
    out.setVersion(QDataStream::Qt_5_8);

    std::string file_path = "../../procon2017-comp/fieldd.csv";
    NeoPolygonIO::exportPolygon(field, file_path);
    std::ifstream input(file_path);
    std::string line_buffer = "";
    QString f;
    while(std::getline(input, line_buffer)){
        f = f + QString::fromStdString(line_buffer);
    }

    quint16 blocksize = f.size();
    out << blocksize << f;
    emit ready();
}

void TcpServer::receive()
{
    quint16 blocksize;
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress("127.0.0.1"), 9999);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_8);

    in >> blocksize;
    QString s;
    in >> s;

    std::string file_path = "../../procon2017-comp/fieldd.csv";
    std::ofstream output(file_path);
    std::string ss = s.toStdString();
    output << ss << std::endl;
}
