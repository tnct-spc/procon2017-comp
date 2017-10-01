#include "qrtranslatetopolygon.h"
#include "neofield.h"
#include <iostream>
#include <string>
#include <vector>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include "neoexpandedpolygon.h"
#include "neopolygonio.h"


QrTranslateToPolygon::QrTranslateToPolygon(std::string qrinp)
{
    inputqr = qrinp;//メンバ変数に代入
    std::cout << qrinp << std::endl;
    inpToVector();
    std::cout << qrinputvector.size() << std::endl;
    piece_size = std::stod(qrinputvector.at(0));
    if(colonvector.size() <= piece_size){
        ismultiqr = true;
        frame_size = -1;
    }else{
        frame_size = colonvector.size() - piece_size;
        ismultiqr = false;
    }
    translateToDoubleArray();
    vectorToPolygon();

    std::cout << "detected!!!!!!!!!!!!!!!!!!!!" << std::endl;
    for(auto output : qrinputvector){
        std::cout << output << " ";
    }
    std::cout << std::endl << "colon!!!!!!!!!!!!!!!!!!" << std::endl;
    for(auto output : colonvector){
        std::cout << output << " ";
    }
    std::cout << std::endl << "piece_size : " << piece_size << "  frame_size : " << frame_size << std::endl;

    std::cout << "PieceData!!!!!!!!!!!" << std::endl;
    for(auto output : pieces){
        std::cout << bg::dsv(output) << std::endl;
    }
    if(getIsMultiQr())std::cout << "MultiQR!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "FrameData!!!!!!!!!!!" << std::endl;
    for(auto output : frames){
        std::cout << bg::dsv(output) << std::endl;
    }
}


void QrTranslateToPolygon::inpToVector(){

    std::string inp;//一時保管するためのstring
    for(unsigned int string_count=0;string_count < inputqr.size();++string_count){
        //実行時に引数として渡されたstringの中身を確認してく
        if(string_count == inputqr.size() - 1){
            inp.push_back(inputqr.at(string_count));
            qrinputvector.push_back(inp);//空白なら代入して中身削除
            inp.clear();
            break;
        }else if(inputqr.at(string_count) == ' '){
            qrinputvector.push_back(inp);//空白なら代入して中身削除
            inp.clear();
        }else if(inputqr.at(string_count) == ':'){

            qrinputvector.push_back(inp);//空白なら代入して中身削除
            inp.clear();
            qrinputvector.push_back(":");
        }else{
            inp.push_back(inputqr.at(string_count));
        }

    }
    for(unsigned int vector_count=0;vector_count<qrinputvector.size();++vector_count){
        if(qrinputvector.at(vector_count) == ":")colonvector.push_back(vector_count);
    }
}

void QrTranslateToPolygon::translateToDoubleArray(){//二重配列に突っ込むやつ
    std::vector<std::vector<int>> inputdatadoublearray;
    std::vector<int> vectorint;//一時的に格納しておくだけ
    for(unsigned int string_count=2;string_count<qrinputvector.size();++string_count){
        if(qrinputvector.at(string_count) == ":"){
            inputdatadoublearray.push_back(vectorint);
            vectorint.clear();
        }else if(string_count == qrinputvector.size() - 1){
            vectorint.push_back(std::stod(qrinputvector.at(string_count)));
            inputdatadoublearray.push_back(vectorint);
        }else{
            vectorint.push_back(std::stod(qrinputvector.at(string_count)));
        }
    }
    //ここからpieceの二重配列とframeの二重配列に入れる作業をする
    for(unsigned int vector_count=0;vector_count<inputdatadoublearray.size();++vector_count){
        if(vector_count < piece_size)piece_data.push_back(inputdatadoublearray.at(vector_count));
        else frame_data.push_back(inputdatadoublearray.at(vector_count));
    }
}

void QrTranslateToPolygon::vectorToPolygon(){

    auto toPolygon = [](std::vector<int> vec_poly){
        polygon_i poly;
        point_i point(0,0);
        for(unsigned int vec_count=1;vec_count<vec_poly.size();++vec_count){
            if(vec_count%2==1){
                point.set<0>(vec_poly.at(vec_count));
            }else{
                point.set<1>(vec_poly.at(vec_count));
                poly.outer().push_back(point);
            }
        }
        poly.outer().push_back(poly.outer().at(0));
        bg::correct(poly);
        return poly;
    };

    for(auto piece : piece_data){
        pieces.push_back(toPolygon(piece));
    }
    for(auto frame : frame_data){
        frames.push_back(toPolygon(frame));
    }
}

std::vector<polygon_i> QrTranslateToPolygon::getPieceData(){
    return pieces;
}

std::vector<polygon_i> QrTranslateToPolygon::getFrameData(){
    return frames;
}
bool QrTranslateToPolygon::getIsMultiQr(){
    return ismultiqr;
}

/*
void QrTranslateToPolygon::findColon()
{
    int cou=0;
    shapecount = std::stod(qrinput);
    qrvector.resize(shapecount);
    for(int tes=0;tes<shapecount;tes++){
        qrvector[tes].resize(128,-1);
    }
    colon.resize(shapecount);
    for(unsigned int tes=0;tes<qrinput.size();++tes){
        if(qrinput[tes]==':'){
            if(cou>=shapecount){
                framestring.at(framedatacount)=qrinput.substr(tes+1);
                useframedata=true;
                ++framedatacount;
            }
            colon[cou]=tes;
            cou++;
        }
    }
}

std::vector<polygon_i> QrTranslateToPolygon::getPieceData()
{
    return polygon;
}

std::vector<polygon_i> QrTranslateToPolygon::getFrameData()
{
    return framepolygon;
}

void QrTranslateToPolygon::splitQrInput()
{
    splitedqrinput.resize(shapecount);
    for(int tes=0;tes<shapecount;tes++){
        splitedqrinput[tes]=qrinput.substr(colon[tes]+1,colon[tes+1]-colon[tes]-1);
    }
}

void QrTranslateToPolygon::splitBasisOfSpace(std::string &str,std::vector<int> &qrvec)
{
        int cou=0;
        FIND:
        for(unsigned int num=0;num<=str.size();num++){
            if(str[num]==' '){
                qrvec[cou]=std::stod(str.substr(0,num));
                str.erase(0,num+1);
                cou++;
                goto FIND;
            }
            if(num == str.size()){
                qrvec[cou]=std::stod(str);
                break;
            }
        }
        qrvec.resize(qrvec[0]*2+1);
}
void QrTranslateToPolygon::translateToPolygon(std::vector<int> &intvec,polygon_i &polygon)
{
    for(unsigned int tes=0;tes<intvec.size()/2;tes++){
        polygon.outer().push_back(point_i(intvec[tes*2+1],intvec[tes*2+2]));
    }
    polygon.outer().push_back(point_i(intvec[1],intvec[2]));
}
*/
void QrTranslateToPolygon::translateToCSV(std::pair<std::vector<polygon_i>, std::vector<polygon_i>> pieceframe, bool is_hint)
{
    procon::NeoField field;
    if(is_hint){
        std::vector<procon::NeoExpandedPolygon> elementaryFrameInnerPieces;
        for(auto ep : pieceframe.first){
            procon::NeoExpandedPolygon elementaryFrameInnerPiece;
            elementaryFrameInnerPiece.resetPolygonForce(ep);
            elementaryFrameInnerPieces.push_back(elementaryFrameInnerPiece);
        }
        field.setElementaryFrameInnerPieces(elementaryFrameInnerPieces);
    }else{
        int id = 0;
        std::vector<procon::NeoExpandedPolygon> elementarypieces;
        for(auto p : pieceframe.first){
            procon::NeoExpandedPolygon pieceexpandedpolygon(id);
            pieceexpandedpolygon.resetPolygonForce(p);
            elementarypieces.push_back(pieceexpandedpolygon);
            id++;
        }

        procon::NeoExpandedPolygon frameexpandedpolygon;
        std::vector<procon::NeoExpandedPolygon> elementaryframe;
        for(auto f : pieceframe.second){
            frameexpandedpolygon.resetPolygonForce(f);
            elementaryframe.push_back(frameexpandedpolygon);
        }

        field.setElementaryPieces(elementarypieces);
        field.setElementaryFrame(elementaryframe);
    }
    NeoPolygonIO::exportPolygon(field, "../../procon2017-comp/fromQRcode.csv");
}

/*
int main()
{
    QrTransToPolygon qrtrans("8:5 7 1 6 5 4 5 0 2 6 0:3 0 0 4 4 0 5:5 2 5 0 5 5 0 5 8 2 8:3 6 2 0 7 0 0:5 6 5 0 0 0 13 9 2 9 5:4 0 0 4 0 4 5 0 3:8 5 1 5 0 7 0 7 3 0 3 0 0 2 0 2 1:4 0 0 3 0 3 3 0 3:9 11 0 11 2 13 2 13 0 16 0 16 10 0 10 0 3 4 0");
    for(unsigned int tes=0;tes<qrtrans.polygon.size();tes++){
        std::cout << "polygon:" << bg::dsv(qrtrans.polygon[tes]) << std::endl;
    }
    std::cout << "frame" << bg::dsv(qrtrans.gon) << std::endl;
    return 0;
}*/
