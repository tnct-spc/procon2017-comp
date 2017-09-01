#include "request.h"

Request::Request(QObject *parent) :
    QObject(parent)
{
    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(sendRequest()));
    connect(&tcpSocket, SIGNAL(disconnected()),
            this, SLOT(closeConnection()));
    connect(&tcpSocket, SIGNAL(readyRead()),
            this, SLOT(updateMessage()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error()));
}

void Request::connectToServer()
{
    tcpSocket.connectToHost(QHostAddress::LocalHost, 6178);
    nextBlockSize = 0;
}

void Request::sendRequest()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8);

    QString string = "Request";
    out << quint16(string.size()) << string;

    tcpSocket.write(block);
}

void Request::updateMessage()
{
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_5_8);

    forever {
        if(nextBlockSize == 0){
            if(tcpSocket.bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize;
        }
        if(nextBlockSize == 0xFFFF){
            tcpSocket.close();
            break;
        }
        if(tcpSocket.bytesAvailable() < nextBlockSize)
            break;

        in >> message;

        nextBlockSize = 0;
    }
}

void Request::closeConnection()
{
    tcpSocket.close();
    emit done();
}

void Request::error()
{
    message = tcpSocket.errorString();
    tcpSocket.close();
    emit done();
}
