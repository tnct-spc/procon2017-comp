#include "tcp_main.h"
#include "request.h"

TcpMain::TcpMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpMain)
{
    ui->setupUi(this);

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

void TcpMain::make_send_data(int piece_number)
{

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
