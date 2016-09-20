#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include <http/help_page.h>
#include <http/answer_form.h>
#include <http/output_problem_page.h>
#include <src/qhttpserver.h>
#include <src/qhttprequest.h>
#include <src/qhttpresponse.h>

class RequestMapper : public QObject
{
    Q_OBJECT
public:
    explicit RequestMapper(QObject *parent = 0);

signals:
    void getAnswer(QString file_path);

private slots:
    void acceptAnswer(QString file_path);
    void service(QHttpRequest* request, QHttpResponse* response);

private:
    HelpPage P_help_page;
    AnswerForm P_answer_page;
    OutputProblemPage P_output_problem_page;
};

#endif // REQUESTMAPPER_H


