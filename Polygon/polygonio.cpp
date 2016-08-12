#include "polygonio.h"

procon::PolygonIO::PolygonIO()
{

}

procon::Field procon::PolygonIO::importPolygon(std::string file_path)
{
    procon::Field field;
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
    ExpandedPolygon ex_flame;
    ex_flame.setPolygon(flame);
    field.setElementaryFlame(ex_flame);

    //pieces
    int pieces_cnt = 0;
    std::vector<procon::ExpandedPolygon> ex_pieces;
    while(std::getline(inputfile, str)){
        polygon_t piece;
        std::string tmpX,tmpY;
        std::istringstream stream(str);
        while(std::getline(stream,tmpX,',')){
            std::getline(stream,tmpY,',');
            piece.outer().push_back(point_t(std::stof(tmpX),std::stof(tmpY)));
        }
        procon::ExpandedPolygon ex_piece;
        ex_piece.setPolygon(piece);
        ex_pieces.push_back(ex_piece);
        pieces_cnt++;
    }
    field.setElementaryPieces(ex_pieces);
    return field;
}

void procon::PolygonIO::exportPolygon(procon::Field field, std::string file_path)
{
    std::ofstream outputfile(file_path);
    //flame
    polygon_t raw_flame = field.getElementaryFlame().getPolygon();
    int flame_size = field.getElementaryFlame().getSize();
    for(int i=0;i<flame_size;i++){
        if(i!=0) outputfile << ",";
        outputfile << raw_flame.outer()[i].x() << "," << raw_flame.outer()[i].y();
    }
    outputfile << "\n";
    //piece
    std::vector<polygon_t> raw_pieces;
    int pieces_size = field.getElementaryPieces().size();
    for(int i=0;i<pieces_size;i++){
        raw_pieces.push_back(field.getElementaryPieces().at(i).getPolygon());
    }
    for(int i=0;i<pieces_size;i++){
        int piece_size = field.getElementaryPieces().at(i).getSize();
        for(int j=0;j<piece_size;j++){
            if(j!=0) outputfile << ",";
            outputfile << raw_pieces.at(i).outer()[j].x() << "," << raw_pieces.at(i).outer()[j].y();
        }
        outputfile << "\n";
    }
    outputfile.close();
}
