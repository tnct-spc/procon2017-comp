#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include "neofield.h"
#include "neopolygonio.h"

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject * parent = 0);
    void getRequest(procon::NeoField input_field);

signals:
    void request();
    void ready();

public slots:
    void newConnection();
    void send();
    void receive();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    procon::NeoField field;
    void setField(procon::NeoField input_field);
    void getFieldCSV();
};

#endif // TCPSERVER_H
