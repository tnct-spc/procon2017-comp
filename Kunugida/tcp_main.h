#ifndef TCP_MAIN_H
#define TCP_MAIN_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QMainWindow>
#include <string>
#include <sstream>
#include <iostream>
#include <QVector>
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
    QVector<std::string> make_send_data_piece();
    QVector<std::string> make_send_data_frame();

protected  slots:
    void send();

private slots:
    void debug();

private:
    Ui::TcpMain *ui;
    QTcpServer *server;
    Request *request;
    procon::NeoField field;
    QPointF getPosition(point_i point);
    std::string getSupportedString(int n, int m);
    const int piece_data = 0;
    const int frame_data = 1;

};

#endif // TCP_MAIN_H
