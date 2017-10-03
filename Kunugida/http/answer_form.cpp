#include "answer_form.h"
#include "neofield.h"
#include "neopolygonio.h"

AnswerForm::AnswerForm(QObject *parent) : QObject(parent)
{
    received_data = "";
}

void AnswerForm::Service(QHttpRequest *request, QHttpResponse *response)
{
    new_response_=response;

    if(request->body().isEmpty()){
        //Wait get body
        connect(request,SIGNAL(data(QByteArray)),this,SLOT(ReceiveData(QByteArray)));
        connect(request,SIGNAL(end()),this,SLOT(ServiceRequestCompleted()));
    }
}

void AnswerForm::ReceiveData(QByteArray lowdata){
    std::cout<<"[http/answer_form] request data receive..."<<std::endl;
    received_data += lowdata;
}

void AnswerForm::ServiceRequestCompleted(){
    std::cout<<"[http/answer_form] request complete!"<<std::endl;
    QByteArray lowdata = received_data;
    received_data = "";

    QHttpResponse *response=new_response_;

    //Get request data
    QUrlQuery url_query(lowdata);
//    QString raw_user_id = url_query.queryItemValue("id");
//    QString raw_answer_data = url_query.queryItemValue("answer");
    //std::cout<<"raw answer data="<<raw_answer_data.toStdString()<<std::endl;

    // Decode
//    raw_user_id.replace("+"," ");
//    raw_user_id.replace("%0A","\n");
//    raw_answer_data.replace("+"," ");
//    raw_answer_data.replace("%0A","\n");
//    QByteArray user_id = raw_user_id.toUtf8();
//    QByteArray answer_data=raw_answer_data.toUtf8();

    // Response head
    response->setHeader("Content-Type", "text/html; charset=UTF-8");
    response->writeHead(200);
    response->write("<html><head><title>ANSWER FORM</title></head><body>");

    // Save answer and emit
    QString filename_posted = "../../procon2017-comp/CSV/posted.csv";
    QString filename_answer = "../../procon2017-comp/CSV/answer.csv";
    QFile answer(filename_posted);
    answer.open(QIODevice::WriteOnly);
    answer.write(lowdata);
    answer.close();

    // compare posted and answer
    procon::NeoField field_posted = NeoPolygonIO::importField(filename_posted.toUtf8().constData());
    procon::NeoField field_answer = NeoPolygonIO::importField(filename_answer.toUtf8().constData());
    if(field_posted.getPiecesSize() > field_answer.getPiecesSize())
        NeoPolygonIO::exportPolygon(field_posted, filename_answer.toUtf8().constData());

    emit getAnswer(filename_answer);

    // Response end
    response->end();
}
