#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

class Request : public QObject
{
    Q_OBJECT

public:
    explicit Request(QObject *parent = 0);
    QString message;

signals:
    void done();

private slots:
    void connectToServer();
    void sendRequest();
    void updateMessage();
    void error();
    void closeConnection();

private:
    QTcpSocket tcpSocket;
    quint16 nextBlockSize;
};

#endif // REQUEST_H
