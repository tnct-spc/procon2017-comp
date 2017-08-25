#ifndef TCP_MAIN_H
#define TCP_MAIN_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QMainWindow>
#include "request.h"
#include "neofield.h"
#include "neoexpandedpolygon.h"

namespace Ui {
class TcpMain;
}
class TcpMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpMain(QWidget *parent = 0);
    ~TcpMain();
    void setfield(procon::NeoField input_field);
    void make_send_data();

protected  slots:
    void send();

private slots:
    void debug();

private:
    Ui::TcpMain *ui;
    QTcpServer *server;
    Request *request;
    procon::NeoField field;

};

#endif // TCP_MAIN_H