#ifndef NEOSLAVER_H
#define NEOSLAVER_H

#include <QWidget>
#include "neoanswerboard.h"
#include "neofield.h"
#include "neopolygonio.h"
#include "neosolver.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QLineEdit>

namespace Ui {
class NeoSlaver;
}

class NeoSlaver : public QWidget
{
    Q_OBJECT

public:
    explicit NeoSlaver(QWidget *parent = 0);
    ~NeoSlaver();

private:
    std::shared_ptr<NeoAnswerBoard> board;
    Ui::NeoSlaver *ui;
    NeoSolver *solver;
    bool get();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool network_error_flag = false;
    void setIPaddress(QString ip_address);

    std::string SAVE_PROBLEM_PATH = "../../procon2017-comp/CSV/problem.csv";
    std::string SAVE_ANSWER_PATH = "../../procon2017-comp/CSV/answer.csv";
    QString SERVER_URL = "http://192.168.100.130:8016/get";
    QString SERVER_POST_URL = "http://192.168.100.130:8016/answer";

    int debug_counter = 1;

private slots:
    void pushGetButton();
    void networkerror(QNetworkReply::NetworkError);
    bool emitAnswer(procon::NeoField);
    bool debugEmitAnswer(procon::NeoField field);
};

#endif // NEOSLAVER_H
