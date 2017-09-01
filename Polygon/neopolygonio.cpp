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
    export2file(4,field.getPieces());
    exportBool2file(5,field.getIsPlaced());
}

procon::NeoField NeoPolygonIO::importField(std::string file_path)
{
    //common
    std::ifstream input(file_path);
    std::string line_buffer = "";
    std::string x, y;
    procon::NeoField import_field;

    //polygon2string
    std::vector<procon::NeoExpandedPolygon> polygons;

    //bool2file
    std::array<bool, 50> is_placed;

    while(std::getline(input,line_buffer)){
        polygon_i hoge;
        std::string point_buffer = "";
        std::istringstream line_stream(line_buffer);
        std::getline(line_stream,point_buffer,',');
        int mode = std::stoi(point_buffer);

        //polygon2string
        if(mode == 0 || mode == 2){
            while(std::getline(line_stream, x, ',')){
                std::getline(line_stream, y, ',');
                hoge.outer().push_back(polygon_i(std::stoi(x), std::stoi(y)));
            }
            int count = 0;
            for(auto &p : hoge){
                procon::NeoExpandedPolygon polygon(count);
                polygon.resetPolygonForce(p);
                polygons.push_back(frame);
                ++count;
            }
            if(mode == 0)
                import_field.setElementaryFrame(polygons);
            if(mode == 2)
                import_field.setElementaryPieces(polygons);
            if(mode == 3)
                import_field.setFrame(polygons);
            if(mode == 4){
                for(auto &p : polygons){
                    import_field.field_pieces.push_back(p);
                }
            }
        }

        //bool2file
        if(mode == 5){
            int js;
            while(std::getline(line_stream, js, ',')){
                if(js == "0"){
                    is_placed = false;
                 }else{
                    is_placed = true;
                }
            }
        }


    }

}


