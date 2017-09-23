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
    qrinput = qrinp;
    findColon();
    splitQrInput();
    for(int tes=0;tes<shapecount;tes++){
        splitBasisOfSpace(splitedqrinput[tes],qrvector[tes]);
    }
    framevector.resize(framedatacount);
    framestring.resize(framedatacount);
    for(auto frame : framevector){
        frame.resize(128,-1);
    }
    if(useframedata){
        for(int count=0;count<framedatacount;++count){
            splitBasisOfSpace(framestring.at(count),framevector.at(count));
        }
    }
    polygon.resize(shapecount);
    for(int tes=0;tes<shapecount;tes++){
        translateToPolygon(qrvector[tes],polygon[tes]);
    }
    if(useframedata){
        for(int count=0;count<framedatacount;++count){
            polygon_i frame_input_polygon;
            translateToPolygon(framevector.at(count),frame_input_polygon);
            framepolygon.push_back(frame_input_polygon);
        }
    }
}

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

void QrTranslateToPolygon::translateToCSV(std::vector<polygon_i> const& piece, std::vector<polygon_i> const& frame)
{
    procon::NeoField field;
    procon::NeoExpandedPolygon pieceexpandedpolygon;
    for(auto p : piece){
        pieceexpandedpolygon.resetPolygonForce(p);
        field.setPiece(pieceexpandedpolygon);
    }
    procon::NeoExpandedPolygon frameexpandedpolygon;
    std::vector<procon::NeoExpandedPolygon> vectorexpandedpolygon;
  //  for(auto frame_ : frame){
  //      frameexpandedpolygon.resetPolygonForce(frame_);
  //      vectorexpandedpolygon.push_back(frameexpandedpolygon);
  //  }
    field.setFrame(vectorexpandedpolygon);
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
