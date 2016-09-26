#include "output_problem_page.h"

OutputProblemPage::OutputProblemPage(QObject *parent) : QObject(parent)
{
    //empty
}

void OutputProblemPage::Service(QHttpRequest *request, QHttpResponse *response){
    std::cout<<"getRequest get with body="<<request->body().data()<<std::endl;
    /*
    //get addless
    QString addless =request->url().toString();

    //saerch filename
    int filelen=addless.length();
    int seek=filelen-1;
    while(addless.mid(seek,1)!="/") seek--;
    */

    //open problem file
    //QString filename_problem=ProblemFolderName+addless.mid(seek);
    QString filename_problem = ProblemFileName;
    QFile problem(filename_problem);
    if(problem.open(QIODevice::ReadOnly)){
        response->writeHead(200);
        response->write(problem.readAll());
        response->end();
        problem.close();
    }else{
        response->writeHead(404);
        //response->write("404 Not Found.");
        response->write("Not yet been generated file.");
        response->end();
    }
    std::cout<<"getRequest end"<<std::endl;
}

