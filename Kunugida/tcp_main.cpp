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
}

QVector<std::string> TcpMain::make_send_data_piece()
{
    //piece or frame(1) + range of vector(2) + QPoint(x(3), y(3))
    QVector<QPointF> points;
    for(auto& piece : field.getPieces()){
        for(auto point : piece.getPolygon().outer()){
            points << getPosition(point);
        }
    }

    QVector<std::string> send_data;
    std::ostringstream ss;
    std::string pointssize = getSupportedString(points.size(), 2);
    for(int i = 0; i < points.size(); ++i){
        std::string xint = getSupportedString(points[i].x(), 3);
        std::string yint = getSupportedString(points[i].y(), 3);
        ss << piece_data << pointssize << xint << yint;
        send_data[i] = ss.str();
    }

    return send_data;
}

QVector<std::string> TcpMain::make_send_data_frame()
{
    //piece or frame(1) + range of vector(2) + QPoint(x(3), y(3))
    QVector<QPointF> frames;
    for(auto& frame : field.getFrame()){
        for(auto point : frame.getPolygon().outer()){
            frames << getPosition(point);
        }
    }

    QVector<std::string> send_data;
    std::ostringstream ss;
    std::string framessize = getSupportedString(frames.size(), 2);
    for(int i = 0; i < frames.size(); ++i){
        std::string xint = getSupportedString(frames[i].x(), 3);
        std::string yint = getSupportedString(frames[i].y(), 3);
        ss << frame_data << framessize << xint << yint;
        send_data[i] = ss.str();
    }

    return send_data;
}

QPointF TcpMain::getPosition(point_i point)
{
    int pointx = point.x();
    int pointy = point.y();
    return QPointF(pointx, pointy);
}

std::string TcpMain::getSupportedString(int n, int m)
{
    std::ostringstream hoge;
    std::string str;
    hoge << n;
    str = hoge.str();
    if(n < 100 && m == 3)
        str = '0' + str;
    if(n < 10 && m <= 3)
        str = '0' + str;
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
