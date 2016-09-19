#include "request_mapper.h"

RequestMapper::RequestMapper(QObject *parent) : QObject(parent){

    QDir().mkdir("docroot");
    QDir().mkdir("docroot/answer");

    QHttpServer *server = new QHttpServer;
    QObject::connect(server, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(service(QHttpRequest*,QHttpResponse*)));

    QObject::connect(&P_answer_page, SIGNAL(getAnswer(QString)), this, SLOT(acceptAnswer(QString)));
    std::cout<<"con"<<std::endl;
    server->listen(8016);
}

void RequestMapper::service(QHttpRequest* request, QHttpResponse* response) {
    std::cout << "getservice"<<std::endl;
    QString path=request->path();

    if (path=="/" || path=="/help") {
        P_help_page.Service(response);
    }
    else if (path=="/answer") {
        P_answer_page.Service(request, response);
    }
    else if (path.startsWith("/get")) {
        P_output_problem_page.Service(request, response);
    }
    else {
        response->writeHead(404);
        response->write("404 Not Found.");
        response->end();
    }
}

void RequestMapper::acceptAnswer(QString file_path)
{
    emit getAnswer(file_path);
}
