#include "neopolygonio.h"

#include <fstream>

#include <boost/range/adaptor/indexed.hpp>


NeoPolygonIO::NeoPolygonIO()
{

}

/*
 * CSVでfieldをファイルに保存したり読み出したりできるようにする
 * 1行あたり1つのNeoExPolygonを表して
 * フレームの内側、すでに置かれたピース等の見分けがつかないのでstateという数字を先頭に設置する
 *
 * 0: 初期でのフレーム
 * 1: 初期でヒントによって配置されたピース
 * 2: 未配置のピース
 * 3: 現時点で結合等を重ねたフレーム
 * 4: 配置済みのピース
 *
 * 一行の中の内容は
 * state,x座標,y座標,...とする
 *
 * これに加えてstd::array<bool,50> is_placedが欲しいので
 * 末行にstate:5で{1 or 0}を50個書いて表現することとする
 */

void NeoPolygonIO::exportPolygon(procon::NeoField field, std::string file_path)
{
    std::ofstream output(file_path);

    auto polygon2string = [](polygon_i polygon)->std::string{
        std::string texted_polygon = "";

        for(const auto& p: polygon.outer() /* | boost::adaptors::indexed() */){
            texted_polygon += ",";
            texted_polygon += std::to_string(p.x());
            texted_polygon += ",";
            texted_polygon += std::to_string(p.y());
        }

        return texted_polygon;
    };

    auto export2file = [&](int state,const std::vector<procon::NeoExpandedPolygon> & polygons){
        for(const auto& p : polygons){
            output << std::to_string(state) << polygon2string(p.getPolygon()) << std::endl;
        }
    };

    auto export2file_with_id = [&](int state,const std::vector<procon::NeoExpandedPolygon> & polygons){
        for(const auto& p : polygons){
            output << std::to_string(state) << "," << std::to_string(p.getId()) << "," << polygon2string(p.getPolygon()) << std::endl;
        }
    };

    auto exportDoubleVector2file = [&](int state,const std::vector<std::vector<procon::NeoExpandedPolygon>> polygonss){
        for (int i = 0; i < polygonss.size(); ++i) {
            for(const auto& p : polygonss[i]){
            output << std::to_string(state) << "," << i << "," << polygon2string(p.getPolygon()) << std::endl;
        }
        }
    };

    auto exportBool2file = [&](int state,const std::array<bool,50>& is_placed){
        output << std::to_string(state);

        for(auto t : is_placed){
            output << "," << t ? "1" : "0";
        }
        output << std::endl;
    };

    export2file(0,field.getElementaryFrame());
    exportDoubleVector2file(1,field.getElementaryFrameInnerPices());
    export2file(2,field.getElementaryPieces());
    export2file(3,field.getFrame());
    export2file_with_id(4,field.getPieces());
    exportBool2file(5,field.getIsPlaced());
}

procon::NeoField NeoPolygonIO::importField(std::string file_path)
{
    //common
    std::ifstream input(file_path);
    std::string line_buffer = "";
    std::string x, y;
    procon::NeoField import_field;
    int mode;

    //polygon2string
    std::vector<procon::NeoExpandedPolygon> elementary_frame;
    std::vector<procon::NeoExpandedPolygon> elementary_piece;
    std::vector<procon::NeoExpandedPolygon> frame;
    std::string id;

    //bool2file
    std::array<bool, 50> is_placed;

    //doublevector2
    std::vector<procon::NeoExpandedPolygon> elementary_frame_inner_pice;
    std::vector<std::vector<procon::NeoExpandedPolygon>> elementary_frame_inner_pices;
    std::string i;

    while(std::getline(input,line_buffer)){
        //elementary_frame_inner_pices[std::stoi(i)] = elementary_frame_inner_pice;
        polygon_i hoge;
        hoge.clear();
        std::string point_buffer = "";
        std::istringstream line_stream(line_buffer);
        std::getline(line_stream,point_buffer,',');
        mode = std::stoi(point_buffer);

        if(mode == 5){
            std::string data;
            int array = 0;
            while(std::getline(line_stream, data, ',')){
                if(data == "0"){
                    is_placed[array] = false;
                }else{
                    is_placed[array] = true;
                }
                ++array;
            }
        }else if(mode == 1){
            std::getline(line_stream, i, ',');
            while(std::getline(line_stream, x, ',')){
                std::getline(line_stream, y, ',');
                hoge.outer().push_back(point_i(std::stoi(x), std::stoi(y)));
            }
            procon::NeoExpandedPolygon polygon;
            polygon.resetPolygonForce(hoge);
            elementary_frame_inner_pice.push_back(polygon);
        }else if(mode == 4){
            std::getline(line_stream, id, ',');
            while(std::getline(line_stream, x, ',')){
                std::getline(line_stream, y, ',');
                hoge.outer().push_back(point_i(std::stoi(x), std::stoi(y)));
            }
            int _id = std::stoi(id);
            procon::NeoExpandedPolygon polygon(_id);
            polygon.resetPolygonForce(hoge);
            import_field.setPiece(polygon);
        }else{
            while(std::getline(line_stream, x, ',')){
                std::getline(line_stream, y, ',');
                hoge.outer().push_back(point_i(std::stoi(x), std::stoi(y)));
            }
            procon::NeoExpandedPolygon polygon;
            polygon.resetPolygonForce(hoge);
            if(mode == 0)
                elementary_frame.push_back(polygon);
            if(mode == 2)
                elementary_piece.push_back(polygon);
            if(mode == 3)
                frame.push_back(polygon);

        }
    }
    import_field.setElementaryFrame(elementary_frame);
    import_field.setElementaryPieces(elementary_piece);
    import_field.setFrame(frame);

    return import_field;
}

