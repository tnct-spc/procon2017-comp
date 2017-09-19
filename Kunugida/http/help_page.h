#ifndef HELPPAGE_H
#define HELPPAGE_H

#include <src/qhttpserver.h>
#include <src/qhttprequest.h>
#include <src/qhttpresponse.h>

class HelpPage : public QObject
{
    Q_OBJECT
public:
    HelpPage(QObject* parent=0);
    void Service(/*QHttpRequest* request,*/ QHttpResponse* response);
};

#endif // HELPPAGE_H
