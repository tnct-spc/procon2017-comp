#ifndef OUTPUTPROBLEMPAGE_H
#define OUTPUTPROBLEMPAGE_H

#include <src/qhttpserver.h>
#include <src/qhttprequest.h>
#include <src/qhttpresponse.h>

class OutputProblemPage : public QObject
{
    Q_OBJECT
public:
    explicit OutputProblemPage(QObject *parent = 0);
    void Service(QHttpResponse* response);

private:
    //pass
    //QString ProblemFolderName=QCoreApplication::applicationDirPath()+"/docroot/files/";
//    QString ProblemFileName = QCoreApplication::applicationDirPath()+"/docroot/problem.csv";
    QString ProblemFileName = "../../procon2017-comp/CSV/Rabbithouse.csv";
};

#endif // OUTPUTPROBLEMPAGE_H
