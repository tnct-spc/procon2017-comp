#include <iostream>
#include <fstream>
#include <sstream>
#include "polygonio.h"

PolygonIO::PolygonIO()
{

}

PolygonSet PolygonIO::importPolygon(std::string file_path)
{
    PolygonSet Set;
    std::ifstream inputfile(file_path);

    std::string str;
    //flame
    polygon_t flame;
    std::getline(inputfile, str);
    std::string tmpX,tmpY;
    std::istringstream stream(str);
    while(std::getline(stream,tmpX,',')){
        std::getline(stream,tmpY,',');
        flame.outer().push_back(point_t(std::stof(tmpX),std::stof(tmpY)));
    }
    Set.fieldFlame.setPolygon(flame);
    //pieces
    int pieces_cnt = 0;
    while(std::getline(inputfile, str)){
        polygon_t piece;
        std::string tmpX,tmpY;
        std::istringstream stream(str);
        while(std::getline(stream,tmpX,',')){
            std::getline(stream,tmpY,',');
            piece.outer().push_back(point_t(std::stof(tmpX),std::stof(tmpY)));
        }
        PolygonExpansion tmpP;
        tmpP.setPolygon(piece);
        Set.fieldPiece.push_back(tmpP);
        pieces_cnt++;
    }

    return Set;
}

void PolygonIO::exportPolygon(PolygonSet set, std::string file_path)
{
    std::ofstream outputfile(file_path);
    //flame
    polygon_t raw_flame = set.fieldFlame.getPolygon();
    int flame_size = set.fieldFlame.size();
    for(int i=0;i<flame_size;i++){
        if(i!=0) outputfile << ",";
        outputfile << raw_flame.outer()[i].x() << "," << raw_flame.outer()[i].y();
    }
    outputfile << "\n";
    //piece
    std::vector<polygon_t> raw_pieces;
    int pieces_size = set.fieldPiece.size();
    for(int i=0;i<pieces_size;i++){
        raw_pieces.push_back(set.fieldPiece.at(i).getPolygon());
    }
    for(int i=0;i<pieces_size;i++){
        int piece_size = set.fieldPiece.at(i).size();
        for(int j=0;j<piece_size;j++){
            if(j!=0) outputfile << ",";
            outputfile << raw_pieces.at(i).outer()[j].x() << "," << raw_pieces.at(i).outer()[j].y();
        }
        outputfile << "\n";
    }
    outputfile.close();
}
