#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "solver.h"
#include "polygonio.h"
#include "answerboard.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QNetworkRequest"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::shared_ptr<AnswerBoard> board;
    Solver* solver;
    Ui::MainWindow *ui;
    bool get();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    bool network_error_flag = false;

    std::string SAVE_PROBLEM_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/problem.csv";
    std::string SAVE_ANSWER_PATH = QCoreApplication::applicationDirPath().toStdString()+"/docroot/answer.csv";
    QString SERVER_URL = "http://127.0.0.1:8016/get";
    QString SERVER_POST_URL = "http://127.0.0.1:8016/answer";

private slots:
    void pushGetButton();
    void networkerror(QNetworkReply::NetworkError);
    bool emitAnswer(procon::Field);
};

#endif // MAINWINDOW_H
