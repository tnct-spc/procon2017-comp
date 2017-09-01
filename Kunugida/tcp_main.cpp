#include "tcp_main.h"
#include "request.h"

TcpMain::TcpMain(QWidget *parent) :
    QMainWindow(parent)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 6178);

    connect(server, SIGNAL(newConnection()), this, SLOT(send()));
    request = new Request();
    connect(request, SIGNAL(done()), this, SLOT(debug()));
}

TcpMain::~TcpMain()
{
    delete ui;
}

void TcpMain::setfield(procon::NeoField input_field)
{
    field = input_field;
    make_send_data_piece();
}

void TcpMain::make_send_data_piece()
{
    //piece or frame(1) + range of vector(2) + QPoint(x(3), y(3))
    QVector<QPointF> points;
    for(auto& piece : field.getPieces()){
        for(auto point : piece.getPolygon().outer()){
            points << getPosition(point);
        }
    }

    QVector<QString> send_data;
    QString pointssize = getSupportedString(points.size(), 2);
    for(int i = 0; i < points.size(); ++i){
        QString xint = getSupportedString(points[i].x(), 3);
        QString yint = getSupportedString(points[i].y(), 3);
        send_data[i] += QString::number(piece_data);
        send_data[i] += pointssize;
        send_data[i] += xint;
        send_data[i] += yint;
    }
    if(!send_data.empty()){
        QMessageBox msgBox;
        msgBox.setText(send_data[0]);
        msgBox.exec();
    }
}

QVector<QString> TcpMain::make_send_data_frame()
{
    //piece or frame(1) + range of vector(2) + QPoint(x(3), y(3))
    QVector<QPointF> points;
    for(auto& frame : field.getPieces()){
        for(auto point : frame.getPolygon().outer()){
            points << getPosition(point);
        }
    }

    QVector<QString> send_data;
    QString pointssize = getSupportedString(points.size(), 2);
    for(int i = 0; i < points.size(); ++i){
        QString xint = getSupportedString(points[i].x(), 3);
        QString yint = getSupportedString(points[i].y(), 3);
        send_data[i] += QString::number(frame_data);
        send_data[i] += pointssize;
        send_data[i] += xint;
        send_data[i] += yint;
    }

    return send_data;
}

QPointF TcpMain::getPosition(point_i point)
{
    int pointx = point.x();
    int pointy = point.y();
    return QPointF(pointx, pointy);
}

QString TcpMain::getSupportedString(int n, int m)
{
    QString str;
    if(m < 100 && n == 3)
        str += QString::number(0);
    if(m < 10){
        if(n == 3)
            str += QString::number(0) + QString::number(0);
        if(n == 2)
            str += QString::number(0);
    }
    str += QString::number(n);

    return str;
}

void TcpMain::send()
{
    QTcpSocket *client = server->nextPendingConnection();
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));

    QDataStream out(client);
    out.setVersion(QDataStream::Qt_5_8);

    QString string = "";
    out << quint16(string.size()) << string << quint16(0xFFFF);
}

void TcpMain::debug()
{
    QMessageBox msgBox(this);
    msgBox.setText(request->message);
    msgBox.exec();
}
