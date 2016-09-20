#include "answer_form.h"

AnswerForm::AnswerForm(QObject *parent) : QObject(parent) {
    // empty
}
void AnswerForm::Service(QHttpRequest *request, QHttpResponse *response) {
    std::cout<<"get answer"<<std::endl;
    new_response_=response;

    if(request->body().isEmpty()){
        //Wait get body
        connect(request,SIGNAL(data(QByteArray)),this,SLOT(ServiceRequestCompleted(QByteArray)));
    }
}

void AnswerForm::ServiceRequestCompleted(QByteArray lowdata){
    QHttpResponse *response=new_response_;

    //Get request data
    QUrlQuery url_query(lowdata);
    QString raw_user_id = url_query.queryItemValue("id");
    QString pre_answer_data = url_query.queryItemValue("answer");
    std::cout<<"predata="<<pre_answer_data.toStdString()<<std::endl;

    //Decode
    raw_user_id.replace("+"," ");
    raw_user_id.replace("%0A","\n");
    pre_answer_data.replace("+"," ");
    pre_answer_data.replace("%0A","\n");
    QByteArray user_id = raw_user_id.toUtf8();
    QByteArray answer_data=pre_answer_data.toUtf8();

    //response head
    response->setHeader("Content-Type", "text/html; charset=UTF-8");
    response->writeHead(200);
    response->write("<html><head><title>ANSWER FORM</title></head><body>");


    /*Save answer and Display answer_point*/
    QString filename_answer=AnswerFolderName+user_id+"_answer.csv";//+user_id+".txt";

    //save answer to Temporary file
    QFile answer(filename_answer);
    answer.open(QIODevice::WriteOnly);
    answer.write(answer_data);
    answer.close();

    emit getAnswer(filename_answer);

    //end
    response->end();
}

QString AnswerForm::SimulateAnswerPoint(QString filename_answer){
/*
    //copy data
    for(int y=0;y<48;y++){
        for(int x=0;x<48;x++){
            stage_state_[y][x]=g_stage_state_[y][x];
        }
    }
    stone_num_=g_stone_num_;
    for(int n=0;n<stone_num_;n++){
        for(int y=0;y<8;y++){
            for(int x=0;x<8;x++){
                stone_state_[n][y][x]=g_stone_state_[n][y][x];
            }
        }
    }



    //アンサー情報のロード
    QFile questionfile(filename_answer);
    questionfile.open(QIODevice::ReadOnly);
    QString line,line2;
    //ファイル解析
    //フィールド情報を配列に格納,フィールドの更新
    int i=0;
    while(1){
        if(questionfile.atEnd()){
            questionfile.close();
            answer_num_=i;
            break;
        }
        //左右
        line = questionfile.read(1);
        line2 = questionfile.read(1);
        if (line2 != " "){
            line = line + line2;
            line2 = questionfile.read(1);
        }
        answer_flow_[i][0] = line.toInt();
        //上下
        line = questionfile.read(1);
        line2 = questionfile.read(1);
        if (line2 != " "){
            line = line + line2;
            line2 = questionfile.read(1);
        }
        answer_flow_[i][1] = line.toInt();
        //裏表
        line = questionfile.read(1);
        if (line == "H") answer_flow_[i][2] = 0;
        if (line == "T") answer_flow_[i][2] = 1;
        line = questionfile.read(1);//空白分
        //角度
        line = questionfile.readLine(8);
        answer_flow_[i][3] = line.toInt();
        i++;
    }
    stone_flow_count_=0;



    //put a stone on stage
    while(PutStone());



    //Simulate point
    int point=0;
    for (int y = 0; y < 32; y++){
        for (int x = 0; x < 32; x++){
            if (stage_state_[8+y][8+x]==2){
                point+=1;
            }
        }
    }

    //return
    return QString::number(point);
*/
    return filename_answer;
}


bool AnswerForm::PutStone(){
    return false;
}
