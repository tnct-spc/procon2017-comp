#include "output_problem_page.h"

OutputProblemPage::OutputProblemPage(QObject *parent) : QObject(parent)
{
}

void OutputProblemPage::Service(QHttpResponse *response)
{
    // Open problem file
    QString filename_problem = ProblemFileName;
    QFile problem(filename_problem);
    if(problem.open(QIODevice::ReadOnly)){
        response->writeHead(200);
        response->write(problem.readAll());
        response->end();
        problem.close();
    }else{
        response->writeHead(404);
        response->write("Not yet been generated file.");
        response->end();
    }
}

