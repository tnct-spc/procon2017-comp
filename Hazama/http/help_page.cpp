#include "help_page.h"

HelpPage::HelpPage(QObject *parent) : QObject(parent) {
    // empty
}

void HelpPage::Service(/*QHttpRequest *request,*/ QHttpResponse *response) {
    response->setHeader("Content-Type", "text/html; charset=UTF-8");
    response->writeHead(200);
    response->write(
                "\n<html>"
                "\n   <body>"
                "\n       TNCT-SPC PROCON27 SERVER<br>"
                "\n       ------------------------<br>"
                "\n       If you want to get the problem. Please access \"/get\".<br>"
                "\n       If you want to send the answer. Please access \"/answer\".<br>"
                "\n   </body>"
                "\n</html>"
                );
    response->end();
}
