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
    bool get();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool network_error_flag = false;

    std::string SAVE_PROBLEM_PATH = "../../procon2017-comp/problem.csv";
    std::string SAVE_ANSWER_PATH = "../../procon2017-comp/answer.csv";
    QString SERVER_URL = "http://192.168.100.55:8016/get";
    QString SERVER_POST_URL = "http://192.168.100.55:8016/answer";

private slots:
    void pushGetButton();
    void networkerror(QNetworkReply::NetworkError);
    bool emitAnswer(procon::NeoField);
};

#endif // NEOSLAVER_H
