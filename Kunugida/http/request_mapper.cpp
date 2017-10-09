#include "request_mapper.h"

RequestMapper::RequestMapper(QObject *parent) : QObject(parent)
{
    QHttpServer *server = new QHttpServer;
    QObject::connect(server, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), this, SLOT(service(QHttpRequest*,QHttpResponse*)));

    QObject::connect(&P_answer_page, SIGNAL(getAnswer()), this, SLOT(acceptAnswer()));

    server->listen(8016);
}

void RequestMapper::service(QHttpRequest* request, QHttpResponse* response)
{
    QString path=request->path();

    if (path=="/" || path=="/help") {
        std::cout << "requested help page"<<std::endl;
        P_help_page.Service(response);
    }
    else if (path=="/answer") {
        std::cout << "requested answer page"<<std::endl;
        P_answer_page.Service(request, response);
    }
    else if (path.startsWith("/get")) {
        std::cout << "requested get page"<<std::endl;
        P_output_problem_page.Service(response);
    }
    else {
        std::cout << "request invalid"<<std::endl;
        response->writeHead(404);
        response->write("404 Not Found.");
        response->end();
    }
}

void RequestMapper::acceptAnswer()
{
    emit getAnswer();
}
