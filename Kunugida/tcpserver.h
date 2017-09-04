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
    void getRequest();

signals:
    void request();

public slots:
    void newConnection();
    void send();

private:
    QTcpServer *server;
    procon::NeoField field;
    void setField(procon::NeoField input_field);
    void getFieldCSV();
};

#endif // TCPSERVER_H
