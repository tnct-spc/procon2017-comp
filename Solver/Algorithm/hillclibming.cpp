#include "hillclibming.h"

HillClibming::HillClibming()
{

}

std::vector<AlgorithmWrapper::PieceAssesment> assesment_function(procon::Field field){
    //many many code
    std::vector<AlgorithmWrapper::PieceAssesment> assesments;
    AlgorithmWrapper::PieceAssesment PA_buf;

    for(auto elementary_piece : field.getElementaryPieces()){
        if(!field.getIsPlaced().at(elementary_piece.getId())){
            for(double x = 0;x < 30;x++){
                for(double y = 0;y < 30;y++){
                    elementary_piece.setPolygonPosition(x,y);
                    elementary_piece.inversePolygon();

                    for(int degree = 0; degree < 360; degree = degree + 10){
                        elementary_piece.setPolygonAngle(degree);

                        if(field.isPuttable(elementary_piece)){
                            PA_buf.EvaluationValue = boost::geometry::area(elementary_piece.getPolygon());
                            PA_buf.Polygon = elementary_piece;
                            assesments.push_back(PA_buf);
                            goto next_roop;
                        }
                    }
                }
            }
        }
        next_roop:;
    }
    return assesments;
}

void HillClibming::run(procon::Field field)
{
    //評価関数の戻り値の格納
    std::vector<PieceAssesment> piece_assesment;
    std::array<bool,50> isPlaced;
    PieceAssesment piece_assesment_buf;

    field.setFrame(field.getElementaryFrame());

    for(auto elementary_piece : field.getElementaryPieces()){

        piece_assesment = assesment_function(field);

        if(piece_assesment.size() == 0){
            break;
        }

        std::sort(piece_assesment.begin(),piece_assesment.end());

        piece_assesment_buf = piece_assesment.at(piece_assesment.size()-1);

        isPlaced = field.getIsPlaced();
        field.setPiece(piece_assesment_buf.Polygon);
        isPlaced.at(piece_assesment_buf.Polygon.getId()) = true;
        field.setIsPlaced(isPlaced);

    }
    submitAnswer(field);
    return;
}
