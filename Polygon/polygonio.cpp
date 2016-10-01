#include "polygonio.h"

procon::PolygonIO::PolygonIO()
{

}

procon::Field procon::PolygonIO::importPolygon(std::string file_path)
{
    procon::Field field;
    std::ifstream inputfile(file_path);

    std::string str;

    //frame
    polygon_t frame;
    std::getline(inputfile, str);
    std::string tmpX,tmpY;
    std::istringstream stream(str);
    while(std::getline(stream,tmpX,',')){ //frame outer
        std::getline(stream,tmpY,',');
        frame.outer().push_back(point_t(std::stof(tmpX),std::stof(tmpY)));
    }
    std::getline(inputfile, str);
    int inner_size = std::stoi(str);
    for(int i=0;i<inner_size;++i){ //frame inners
        std::getline(inputfile, str);
        std::string tmpX,tmpY;
        std::istringstream stream(str);
        frame.inners().push_back(polygon_t::ring_type());
        while(std::getline(stream,tmpX,',')){
            std::getline(stream,tmpY,',');
            frame.inners().at(i).push_back(point_t(std::stof(tmpX),std::stof(tmpY)));
        }
    }
    ExpandedPolygon ex_frame;
    ex_frame.resetPolygonForce(frame);
    field.setElementaryFrame(ex_frame);

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
        procon::ExpandedPolygon ex_piece(pieces_cnt);
        ex_piece.resetPolygonForce(piece);
        ex_pieces.push_back(ex_piece);
        pieces_cnt++;
    }
    field.setElementaryPieces(ex_pieces);
    return field;
}

void procon::PolygonIO::exportPolygon(procon::Field field, std::string file_path)
{
    std::ofstream outputfile(file_path);

    //frame outer
    procon::ExpandedPolygon raw_ex_frame = field.getElementaryFrame();
    polygon_t raw_frame = raw_ex_frame.getPolygon();
    int frame_size = raw_ex_frame.getSize();
    for(int i=0;i<frame_size;i++){
        if(i!=0) outputfile << ",";
        outputfile << raw_frame.outer()[i].x() << "," << raw_frame.outer()[i].y();
    }
    outputfile << "\n";

    //frame inner
    outputfile << raw_ex_frame.getInnerSize() << "\n";
    for(auto inner : raw_frame.inners()){

        bool line_begin_flag = true;

        for(auto corner : inner){

            if(line_begin_flag){
                line_begin_flag = false;
            }else{
                outputfile << ",";
            }

            outputfile << corner.x() << "," << corner.y();
        }
        outputfile << "\n";
    }

    //piece
    std::vector<polygon_t> raw_pieces;
    int pieces_size = field.getElementaryPieces().size();
    for(int i=0;i<pieces_size;i++){
        raw_pieces.push_back(field.getElementaryPieces().at(i).getPolygon());
    }
    for(int i=0;i<pieces_size;i++){
        procon::ExpandedPolygon raw_ex_piece = field.getElementaryPieces().at(i);
        int piece_size = raw_ex_piece.getSize();
        for(int j=0;j<piece_size+1;j++){ //becareful output +1 corner!
            if(j!=0) outputfile << ",";
            outputfile << raw_pieces.at(i).outer()[j].x() << "," << raw_pieces.at(i).outer()[j].y();
        }
        outputfile << "\n";
    }
    outputfile.close();
}

void procon::PolygonIO::exportAnswer(procon::Field field, std::string file_path)
{
    std::ofstream outputfile(file_path);

    for(auto& piece : field.getFrame().getJointedPieces()){
        outputfile << piece.getId() << ","
                   << piece.centerx << ","
                   << piece.centery << ","
                   << piece.difference_of_default_degree << ","
                   << piece.is_inverse << "\n";
    }
    outputfile.close();
}

procon::Field procon::PolygonIO::importAnswer(std::string file_path, procon::Field field)
{
    std::vector<procon::ExpandedPolygon> pieces;

    std::ifstream inputfile(file_path);

    std::string str;
    while(std::getline(inputfile, str)){
        std::istringstream stream(str);

        std::string tmpID,tmpX,tmpY,tmpR,tmpI;

        while(std::getline(stream,tmpID,',')){
            std::getline(stream,tmpX,',');
            std::getline(stream,tmpY,',');
            std::getline(stream,tmpR,',');
            std::getline(stream,tmpI,',');

            procon::ExpandedPolygon tmpPolygon = field.getElementaryPieces().at(std::stoi(tmpID));
            tmpPolygon.setPolygonPosition(std::stod(tmpX),std::stod(tmpY));
            if(std::stoi(tmpI) == true) tmpPolygon.inversePolygon();
            tmpPolygon.setPolygonAngle(std::stod(tmpR));
            pieces.emplace_back(tmpPolygon);
        }
    }

    procon::ExpandedPolygon frame = field.getElementaryFrame();
    frame.replaceJointedPieces(pieces);
    field.setFrame(frame);

    return field;
}
