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

void TcpMain::make_send_data()
{

    //piece or frame(1) + range of vector(2) + number of vertices(6) + QPoint(x(3), y(3))
    //example: 0 + 49 + 000006 + 9032 -> 0490000069032
    const int piece_data = 0;
    const int frame_data = 1;
    std::vector<QPointF> points;
    for(auto pieces_data_size : field.getPieces().size()){
        for(auto point : field.getPieces().at(pieces_data_size).getPolygon().outer()){
            points.push_back(point);
        }
    }
    std::vector<>



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
