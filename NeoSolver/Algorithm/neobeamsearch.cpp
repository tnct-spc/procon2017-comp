#include "neobeamsearch.h"
#include "Evaluation/evaluation.h"

std::mutex mtex;

//PolygonConnector用の
typedef std::tuple<std::vector<procon::NeoExpandedPolygon>, procon::NeoExpandedPolygon, bool> ConnectedResult;


NeoBeamSearch::NeoBeamSearch()
{
    logger = spdlog::get("BeamSearch");
//    logger = spdlog::get("beamsearch");
    dock = std::make_shared<NeoAnswerDock>();
    dock->show();

    //test
    neo = std::make_shared<NeoAnswerDock>();
    neo->show();

    cpu_num = std::thread::hardware_concurrency();
}

NeoBeamSearch::NeoBeamSearch(int beamwidth)
{
    this->setBeamWidth(beamwidth);

    logger = spdlog::get("BeamSearch");
//    logger = spdlog::get("beamsearch");
    dock = std::make_shared<NeoAnswerDock>();
    dock->show();

    //test
    neo = std::make_shared<NeoAnswerDock>();
    neo->show();

    cpu_num = std::thread::hardware_concurrency();
}

NeoBeamSearch::NeoBeamSearch(int beamwidth, bool answerprogress_enabled)
{
    this->setBeamWidth(beamwidth);

    this->answer_progress_enabled = answerprogress_enabled;

    logger = spdlog::get("BeamSearch");
//    logger = spdlog::get("beamsearch");
    dock = std::make_shared<NeoAnswerDock>();
    neo = std::make_shared<NeoAnswerDock>();
//    dock->setSelectAnswerMode(true);


    if(answer_progress_enabled){
        neo->show();
        dock->show();
    }

    last_selector = std::make_shared<NeoAnswerDock>();
    last_selector->setSelectAnswerMode(true);

    connect(last_selector.get(),&NeoAnswerDock::selectedAnswer,[&](procon::NeoField field){
        this->submitAnswer(field);
    });

    cpu_num = std::thread::hardware_concurrency();
}

void NeoBeamSearch::setBeamWidth(int beamwidhth)
{
    this->beam_width = beamwidhth;
}

std::string NeoBeamSearch::hashField(const procon::NeoField& field){
    std::vector<procon::NeoExpandedPolygon> polygons = field.getFrame();
    std::vector<procon::NeoExpandedPolygon> frame = field.getElementaryFrame();
    //polygonのvectorをすべてpointのvectorにまとめる
    auto polygons_to_points=[](std::vector<procon::NeoExpandedPolygon> vectorA){
        std::vector<point_i> v;
        for(procon::NeoExpandedPolygon i : vectorA){
            polygon_i polygon1 = i.getPolygon();
            polygon1.outer().pop_back();
            for(point_i j : polygon1.outer()){
                v.push_back(j);
            }
        }
        return v;
    };
    //pointのvectorをソートする
    auto sort_points=[](std::vector<point_i> vectorA){
        sort(vectorA.begin(),
             vectorA.end(),
             [](point_i a,point_i b){
                if(a.x()==b.x()){
                    return (a.y() < b.y());
                }
                return (a.x() < b.x());
             }
        );
        return vectorA;
    };
    //int型10進数をstring型16進数に変換する
    auto int_to_string=[](unsigned int val,bool lower = true){
        if( !val )
                return std::string("0");
            std::string str;
            const char hc = lower ? 'a' : 'A';     // 小文字 or 大文字表記
            while( val != 0 ) {
                int d = val & 15;     // 16進数一桁を取得
                if( d < 10 )
                    str.insert(str.begin(), d + '0');  //  10未満の場合
                else //  10以上の場合
                    str.insert(str.begin(), d - 10 + hc);
                val >>= 4;
            }
            return str;
    };
    //pointのvectorを文字列に変換する
    auto points_to_string=[int_to_string](std::vector<point_i> vectorA){
        std::string string;
        for(point_i i : vectorA){
            string+=int_to_string(i.x());
            string+=int_to_string(i.y());
        }
        return string;
    };

    //polygonのvectorをpointにしてソートする
    std::vector<point_i> polygons_points_vector=polygons_to_points(polygons);
    polygons_points_vector=sort_points(polygons_points_vector);

    //frameのouterのpointをソートする
    std::vector<point_i> frame_points_vector;
    for(procon::NeoExpandedPolygon i : frame){
        polygon_i polygon1 = i.getPolygon();
        for(point_i j : polygon1.outer()){
            frame_points_vector.push_back(j);
        }
    }
    frame_points_vector=sort_points(frame_points_vector);

    std::string answer;
    answer+=points_to_string(polygons_points_vector);
    answer+="_";
    answer+=points_to_string(frame_points_vector);
    return answer;
}

void NeoBeamSearch::makeNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    std::vector<procon::NeoField> next_field;

    //mulutithread mode
    int evaluate_counter = 0;

    auto makeNextFieldFromEvaluate = [&](){

        while(true){

            Evaluate eval;

            {
                std::lock_guard<decltype(mtex)> lock(mtex);
                if(evaluations.empty()){
                    return;
                }else if(next_field.size() >= beam_width){
                    return;
                }

                eval = evaluations[evaluations.size() - 1];
                evaluations.pop_back();

                ++evaluate_counter;
                logger->info("evaluate counter:" + std::to_string(evaluate_counter));
            }

            logger->info("evaluating");

            procon::NeoField field_buf;
            {
                std::lock_guard<decltype(mtex)> lock(mtex);
                field_buf = fields[eval.fields_index];
            }
            ConnectedResult connect_result = PolygonConnector::connect(field_buf.getFrame()[eval.frame_index]
                    ,eval.is_inversed
                        ? field_buf.getElementaryInversePieces()[eval.piece_index]
                        : field_buf.getElementaryPieces()[eval.piece_index]
                    ,eval.connection
            );

            if(std::get<2>(connect_result)){
                std::array<bool,50> is_placed = field_buf.getIsPlaced();
                is_placed[eval.piece_index] = true;
                field_buf.setIsPlaced(is_placed);

                field_buf.setPiece(std::get<1>(connect_result));

                std::vector<procon::NeoExpandedPolygon> frames_buf = field_buf.getFrame();
                frames_buf.erase(frames_buf.begin() + eval.frame_index);
                for (auto frame_polygon : std::get<0>(connect_result)){
                    frames_buf.push_back(frame_polygon);
                }

                field_buf.setFrame(frames_buf);

                //deplicacte flag
                bool flag = false;

                //check field is ok
                {
                    const std::string now_hash = hashField(field_buf);
                    std::lock_guard<decltype(mtex)> lock(mtex);
                    std::for_each(next_field.begin(),next_field.end(),[&](const procon::NeoField& f){
                        if(!flag){
                            if(now_hash == hashField(f)){
                                flag = true;
                            }
                        }
                    });
                }

                if(!flag){
                    if(!this->checkCanPrune(field_buf)){
                        {
                            std::lock_guard<decltype(mtex)> lock(mtex);
                            field_buf.evaluate_cache.push_back(eval);
                            next_field.push_back(field_buf);
                        }
                    }
                }
            }
        }
    };


    std::sort(evaluations.begin(),evaluations.end(),[](Evaluate l,Evaluate r){
        return l.score < r.score;
    });

    std::vector<std::thread> threads(cpu_num);
    for(auto& th : threads){
        th = std::thread(makeNextFieldFromEvaluate);
    }

    for(auto& th : threads){
        th.join();
    }

    logger->info("thread joined");

    fields.clear();
    std::copy(next_field.begin(),next_field.end(),std::back_inserter(fields));
}

bool NeoBeamSearch::checkCanPrune(const procon::NeoField &field)
{
    clock_t a1,a2,c1,c2,d1,d2,e1,e2;

    //角について枝切りできるかできないか
    auto about_angle = [&field](){


        double hoge_frame_angle;
        double min_frame_angle = 360;
        for(auto const &frame : field.getFrame()){
            for(int angle = 0; angle < frame.getSize(); ++angle){
                hoge_frame_angle = frame.getSideAngle().at(angle);
                if(hoge_frame_angle < min_frame_angle) min_frame_angle = hoge_frame_angle;
            }
        }
//        std::cout << "枠の最小角は " << min_frame_angle << std::endl;

        double hoge_piece_angle;
        double min_piece_angle = 360;
        for(auto const &piece : field.getElementaryPieces()){
            for(int pieceangle = 0; pieceangle < piece.getSize(); ++pieceangle){
                hoge_piece_angle = piece.getSideAngle().at(pieceangle);
                if(hoge_piece_angle < min_piece_angle) min_piece_angle = hoge_piece_angle;
            }
        }
//        std::cout << "ピースの最小角は " <<min_piece_angle << std::endl;

        if(min_frame_angle < min_piece_angle){
            return true;
        }else{
            return false;
        }
    };
    //辺について枝きりできるかできないか
    /*auto about_side = [&field](){
        //要素数の足し算、引き算
        auto calculation_nep = [](const procon::NeoExpandedPolygon &nep , int index , int cal){
            index = index + cal;
            index = index % nep.getSize();
            if(index < 0){
                index = nep.getSize() + index;
            }
            return index;
        };
        auto calculation_rad = [](double a){
            //ラジアン(radian)から角度(degree)に変換
            return a * 180.0 / 3.141592653589793;
        };

        //frameの辺を大きい順にならべる
        std::vector<std::tuple<double , double , double>> frame_sides;
        for(procon::NeoExpandedPolygon frame : field.getFrame()){
            for(int i = 0 ;i < frame.getSize() ; i++){
                frame_sides.push_back(std::tuple<double , double , double>(
                                          frame.getSideLength().at(i),
                                          frame.getSideAngle().at(i),
                                          frame.getSideAngle().at(calculation_nep(frame,i,1))
                                          )
                                 );
            }
        }
        sort(frame_sides.begin() , frame_sides.end() , [](auto a , auto b){
            if(std::get<0>(a) != std::get<0>(b)){
                return (std::get<0>(a) > std::get<0>(b));
            }else if(std::get<1>(a) != std::get<1>(b)){
                return (std::get<1>(a) > std::get<1>(b));
            }else{
                return std::get<2>(a) > std::get<2>(b);
            }
        });
        for(std::tuple<double , double , double> i : frame_sides){
            std::cout << std::get<0>(i) << " " << calculation_rad(std::get<1>(i))<<" "<<calculation_rad(std::get<2>(i))<<std::endl;
        }

        std::cout<<std::endl;

        //piecesの辺を小さい順にならべる
        std::vector<double> pieces_sides;
        for(int i = 0 ; i < field.getElementaryPieces().size() ; i++){
            if(field.getIsPlaced().at(i)) continue;
            procon::NeoExpandedPolygon piece = field.getElementaryPieces().at(i);
            for(int i = 0 ; i < piece.getSize() ; i++){
                pieces_sides.push_back(piece.getSideLength().at(i));
            }
        }
        sort(pieces_sides.begin() , pieces_sides.end() , [](auto a , auto b){
            return (a < b);
        });
        for(double i : pieces_sides){
            std::cout << i << std::endl;
        }

        bool size_only = std::get<0>(frame_sides.at(0)) < pieces_sides.at(0);
        bool angles = (std::get<1>(frame_sides.at(0)) < M_PI) || (std::get<2>(frame_sides.at(0)) < M_PI);
        return size_only && angles;
    };
    */
    //対角線で枝きりできるか否か
    auto about_distance = [&field](){
        struct tool{                //関数内関数のための構造体
            static int Distance(point_i point_a,point_i point_b){ //2点を引数にその間の距離の二乗を返す
                int a = point_a.x();
                int b = point_a.y();
                int c = point_b.x();
                int d = point_b.y();
                int distance = (a - c)*(a - c) + (b - d)* (b - d);
                return distance;
            }
            static int polygondistance(polygon_i polygon){   //ポリゴンを引数にとってその中で一番長い辺or対角線の二乗をかえす。
                int distance = 0,instance;
                for(point_i point_a : polygon.outer()){
                    for(point_i point_b : polygon.outer()){
                        instance = Distance(point_a,point_b);
                        if(instance > distance){
                            distance = instance;
                        }
                    }
                }
                return distance;
            }
        };
        int most_frame = 0,most_piece = 0,frame_instance,piece_instance;
        for(auto const neoframe :field.getFrame()){
             polygon_i frame =neoframe.procon::NeoExpandedPolygon::getPolygon();
             frame_instance = tool::polygondistance(frame);
             if(most_frame < frame_instance){
                 most_frame = frame_instance;
             }
        }
        for(auto const neopiece : field.getPieces()){
            polygon_i piece = neopiece.procon::NeoExpandedPolygon::getPolygon();
            piece_instance = tool::polygondistance(piece);
            if(most_piece < piece_instance){
                most_piece = piece_instance;
            }
        }
        if(most_piece > most_frame){
//            std::cout <<"対角線で枝きり"<<std::endl;
            return true;
        }
//        std::cout <<"対角線では無理だった"<<std::endl;
        return false;
    };


    auto framesize_single = [&field](procon::NeoExpandedPolygon frame){ //一つのフレームとピースとの面積が合致するかを出す関数    時間に対して弾けるパターンが少なすぎるので改善しましょう
        /*
        std::vector<double> area_vec;
        const double frame_area = bg::area(frame.getPolygon());
        std::cout << field.getElementaryPieces().size() << field.getFrame().size() << std::endl;
        for(auto piece : field.getElementaryPieces()){
            double area = bg::area(piece.getPolygon());
            std::cout << "frame_area : " << frame_area << "  piece_area : " << area << std::endl;
            if(area < frame_area)area_vec.push_back(area);//面積を片っ端から代入
            else if(area == frame_area)return false;//このframeに関しては問題ない
        }
        std::cout << "area_vec.size : " << area_vec.size() << std::endl;
        if(area_vec.size() == 0)return true;//問題があるのでtrue返して終了
        std::sort(area_vec.begin(),area_vec.end());


        std::vector<double> add_vec = area_vec;
        for(unsigned int count = 1;count < add_vec.size();++count){
            std::cout << "area_vec.size : " << area_vec.size() << "   add_vec.size : " << add_vec.size() << std::endl;
            for(auto area : area_vec){
                for(unsigned int vec_count=0;vec_count<add_vec.size();++vec_count){
                    std::cout << "どうでしょう" << vec_count << std::endl;
                    double add_cou = area + add_vec.at(vec_count);
                    if(add_cou < frame_area)add_vec.push_back(add_cou);
                    else if(add_cou == frame_area){

                        std::cout << "add_vec一覧表示 : ";
                        for(auto count : add_vec){
                            std::cout << count << " ";
                        }
                        std::cout << std::endl;
                        return false;
                    }
                    else break;
                }
            std::sort(add_vec.begin(),add_vec.end());
            add_vec.erase(std::unique(add_vec.begin(),add_vec.end()) , add_vec.end());
            }
        }
        return true;*/


        std::vector<double> piecearea_vec;
        for(auto piece : field.getElementaryPieces()){
            piecearea_vec.push_back(bg::area(piece.getPolygon()));
//            std::cout << bg::area(piece.getPolygon());
        }

//        std::cout << field.getElementaryPieces().size() << std::endl;
        const int piece_cou = field.getElementaryPieces().size();
        for(int cou=0;cou < std::pow(2,piece_cou );++cou){
            double add_number = 0;//ここの数値に対応する値を加算していく
            for(int digit=0;digit<piece_cou;++digit){
                int count = std::pow(2 , digit);//pieceの合計が3つなら4,2,1みたいな感じのが出る(100,010,001みたいになる)
                if(count & cou){//100,101みたいな感じなら100が返ってくるからn桁目が0か1かが分かる
                    add_number += piecearea_vec.at(digit);
                }
            }
//            std::cout << add_number << "  " << bg::area(frame.getPolygon()) << std::endl;
           // std::cout << std::pow(2,piece_cou) << " " << cou << std::endl;
            if(add_number == bg::area(frame.getPolygon()))return false;
        }


        return true;
    };

    //複数のFrameがあるときにピースと面積が合致するか
    auto about_framesize = [&field,&framesize_single](){
        const int frame_size_max = 1000;//これより大きい面積のframeは判定しない(処理に時間がかかるため)
//        std::cout << "frame_size : " << field.getFrame().size() << std::endl;
        for(auto frame : field.getFrame()){
            if(bg::area(frame.getPolygon()) < frame_size_max){
                if(framesize_single(frame)){
//                    std::cout << "問題の原因になったframe : " << bg::dsv(frame.getPolygon()) << std::endl;
                    return true;
                }
            }
        }
        return false;
    };



    //複数のframeがある時にその内角を満たす角の組み合わせが存在するか調べる
    auto about_frameangle = [&field](){
        const int frame_angle_max = 180;
        auto calculation_rad = [](double a){
            //ラジアン(radian)から角度(degree)に変換
            return a * 180.0 / 3.141592653589793;
        };

        std::vector<double> frameangle_vec;
        std::vector<double> add_vec;

        auto erase_frame = [&frameangle_vec,&add_vec](double angle){
                for(int count=0;count<frameangle_vec.size();++count){
                    double frame_angle = frameangle_vec.at(count);
                    if(frame_angle < angle + 0.01 && frame_angle > angle - 0.01){
                        frameangle_vec.erase(frameangle_vec.begin() + count,frameangle_vec.end());
//                        std::cout << "frameangle_vec.size : " << frameangle_vec.size() << std::endl;
                        if(frameangle_vec.size())return true;
                        break;
                    }
                }
                return false;
        };

        auto erase_frame_vec = [&frameangle_vec,&add_vec](){
            bool flag;
            while(!flag){
                flag=true;
                for(int count=0;count<frameangle_vec.size();++count){
                    double frame_angle = frameangle_vec.at(count);
                    for(auto angle : add_vec){
//                        std::cout << "frame_angle : " << frame_angle << "  " << angle << std::endl;
                        if(frame_angle < angle + 0.01 && frame_angle > angle - 0.01){
//                            std::cout << "かぶってます" << frame_angle << "  " << angle << std::endl;
                            frameangle_vec.erase(frameangle_vec.begin() + count,frameangle_vec.end());
//                            std::cout << "frameangle_vec.size : " << frameangle_vec.size() << std::endl;
                            flag=false;
                            break;
                        }else if(frame_angle < angle + 0.01)break;
                    }
                }
            }
            if(frameangle_vec.size()==0)return true;
            return false;
        };


        std::vector<double> pieceangle_vec;
        for(auto frame : field.getFrame()){

            for(auto side_angle_ : frame.getSideAngle()){
                double side_angle = calculation_rad(side_angle_);
//                std::cout << side_angle << std::endl;
                if(side_angle < frame_angle_max)frameangle_vec.push_back(side_angle);
            }
        }
        std::sort(frameangle_vec.begin(),frameangle_vec.end());
        frameangle_vec.erase(std::unique(frameangle_vec.begin(),frameangle_vec.end()),frameangle_vec.end());
        for(auto piece : field.getElementaryPieces()){
            for(auto side_angle_ : piece.getSideAngle()){
                double side_angle = calculation_rad(side_angle_);
                pieceangle_vec.push_back(side_angle);
            }
        }

        std::cout << "frame_vec一覧表示 : ";
        for(auto angle : frameangle_vec){
//            std::cout << angle << " ";
        }
        std::cout << std::endl;

        std::sort(pieceangle_vec.begin(),pieceangle_vec.end());
        for(int count=0;count<pieceangle_vec.size();++count){
           if(pieceangle_vec.at(count) >= frame_angle_max){
               pieceangle_vec.erase(pieceangle_vec.begin() + count,pieceangle_vec.end());
               break;
           }
        }
        pieceangle_vec.erase(std::unique(pieceangle_vec.begin(),pieceangle_vec.end()),pieceangle_vec.end());

        add_vec = pieceangle_vec;
        if(erase_frame_vec())return false;

//        std::cout << "frame_vec一覧表示 : ";
//        for(auto angle : frameangle_vec){
//            std::cout << angle << " ";
//        }
//        std::cout << std::endl;

//        std::cout << "piece_vec一覧表示 : ";
//        for(auto angle : pieceangle_vec){
//            std::cout << angle << " ";
//        }
//        std::cout << std::endl;


        bool flag= false;
        for(unsigned int count=0;count<pieceangle_vec.size();++count){
            for(int add_count=0;add_count<add_vec.size();++add_count){
                bool check=false;
                for(auto piece_angle : pieceangle_vec){
                    double add_number = add_vec.at(add_count) + piece_angle;
                    if(add_number < frame_angle_max){
                        add_vec.push_back(add_number);
                        if(erase_frame(add_number))return false;
                        check=true;
                    }
                    else break;
                }
                if(!check){//ここまで行けばループ脱出
                    flag=true;
                    break;
                }
                std::sort(add_vec.begin(),add_vec.end());
                add_vec.erase(std::unique(add_vec.begin(),add_vec.end()),add_vec.end());
                //if(erase_frame_vec())return false;
            }
            if(flag)break;
        }

//        std::cout << "frame_vec一覧表示 : ";
//        for(auto angle : frameangle_vec){
//            std::cout << angle << " ";
//        }
//        std::cout << std::endl;

//        std::cout << "piece_vec一覧表示 : ";
//        for(auto angle : pieceangle_vec){
//            std::cout << angle << " ";
//        }
//        std::cout << std::endl;

//        std::cout << "add_vec一覧表示 : ";
//        for(auto angle : add_vec){
//            std::cout << angle << " ";
//        }
        std::cout << std::endl;

        if(erase_frame_vec())return false;

        return true;
    };
//    bool a = about_distance();
//    if(a)return a;
//    bool b = about_angle();
//    if(b)return b;
//    bool c = about_framesize();
//    if(c)return c;
//    bool d = about_frameangle();
//    if(d)return d;
    return false;
    //OKならfalseを返す
}

int NeoBeamSearch::checkOddField(const procon::NeoField &field, const Connect &connector, const int field_frame_index, const int field_piece_index) //field_frame_indexはConnectクラスのindexで参照すべきframeのindex
{
    //mag = magnification（倍率）
    int odd_score = 0; //ゲテモノポイント
    const int odd_connect_length = 3; //これ以下の長さだとゲテモノ（単位はグリット）
    const int check_connect_length_mag = 10; //１倍のときのゲテモノポイント加算値はodd_connect_length以下の整数になる

    std::vector<procon::NeoExpandedPolygon> frames = field.getFrame();

    auto check_connect_length = [&]() //結合した辺のゲテモノ度
    {
        procon::NeoExpandedPolygon frame = frames.at(static_cast<unsigned int>(field_frame_index));
        std::vector<double> frame_lengthes = frame.getSideLength();
        double frame_length = frame_lengthes.at(static_cast<unsigned int>(connector.frame_side_index));

        auto checklength = [](double length)
        {
            double length_int;
            double length_double = modf(length, &length_int);
            return static_cast<int>((length_double > 0.0) ? length_int + 1.0 : length_int);
        };

        int frame_length_up_int = checklength(frame_length);
        if(frame_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - frame_length_up_int) * check_connect_length_mag;
        else {
            procon::NeoExpandedPolygon piece = field.getPiece(field_piece_index);
            std::vector<double> piece_lengthes = piece.getSideLength();
            double piece_length = piece_lengthes.at(static_cast<unsigned int>(connector.polygon_side_index));
            int piece_length_up_int = checklength(piece_length);
            if(piece_length_up_int <= odd_connect_length) odd_score += (odd_connect_length + 1 - piece_length_up_int) * check_connect_length_mag;
        }
    };

    if(odd_connect_length > 0 && check_connect_length_mag > 0) check_connect_length();

    return odd_score;
}

void NeoBeamSearch::evaluateNextState(std::vector<procon::NeoField> & fields,std::vector<Evaluate> & evaluations)
{
    //frameがstd::vector<NeoExPolygon>なのでそれぞれに対して、評価関数を回す
    auto evaluateWrapper = [&](procon::NeoField const& field,int const& piece_index,int const& fields_index){
        int frame_index = 0;
        for(const auto& f : field.getFrame()){
            //inverseしていない方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate = Evaluation::evaluation(f,field.getElementaryPieces()[piece_index],1.0,1.0,true);
            //inverseしている方のpiece評価
            std::vector<std::pair<double,Connect>> evaluate_inversed = Evaluation::evaluation(f,field.getElementaryInversePieces()[piece_index],1.0,1.0,true);

            //一時保存用の変数
//            TODO: いい感じにここをパフォーマンスよくする
            Evaluate ev_buf;
            for(const auto& e : evaluate){
                if(e.first){
                    ev_buf.score = e.first;
                    ev_buf.connection = e.second;
                    ev_buf.fields_index = fields_index;
                    ev_buf.frame_index = frame_index;
                    ev_buf.piece_index = piece_index;
                    ev_buf.is_inversed = false;

                    {
                        std::lock_guard<decltype(mtex)> lock(mtex);
                        evaluations.push_back(ev_buf);
                    }

                }
            }
            for(const auto& e : evaluate_inversed){
                if(e.first){
                    ev_buf.score = e.first;
                    ev_buf.connection = e.second;
                    ev_buf.fields_index = fields_index;
                    ev_buf.frame_index = frame_index;
                    ev_buf.piece_index = piece_index;
                    ev_buf.is_inversed = true;

                    {
                        std::lock_guard<decltype(mtex)> lock(mtex);
                        evaluations.push_back(ev_buf);
                    }
                }
            }

            ++frame_index;
        }
    };


    int global_field_index = 0;

    auto evaluateNextState = [&](){
        while(true){

            procon::NeoField field_buf;
            int now_field_index = 0;

            {
                std::lock_guard<decltype(mtex)> lock(mtex);

                if(global_field_index == fields.size()){
                    return;
                }

                logger->info("evaluating " + std::to_string(global_field_index));

                field_buf = fields.at(global_field_index);
                now_field_index = global_field_index;

                ++global_field_index;
            }

            for (int piece_index = 0; piece_index < field_buf.getElementaryPieces().size(); ++piece_index) {
                //すでに置いてあったら評価しません
                if(field_buf.getIsPlaced().at(piece_index)) continue;

                evaluateWrapper(field_buf,piece_index,now_field_index);
            }
        }
    };

    std::vector<std::thread> threads(cpu_num);
    for(auto& th : threads){
        th = std::thread(evaluateNextState);
    }

    for(auto& th : threads){
        th.join();
    }
}

void NeoBeamSearch::init()
{
#ifdef DEBUG_MODE
//    logger->info("debug mode");
#else
    logger->info("efficient mode");
#endif
}
void NeoBeamSearch::run(procon::NeoField field)
{
    logger->info("beamsearch run");

    //時間計測
    std::chrono::system_clock::time_point start,end;
    start = std::chrono::system_clock::now();

    dock->addAnswer(field);
    logger->info("beamsearch run");

    std::vector<procon::NeoField> state;

    state.push_back(field);

//    ev.resize(2000000);
    for (int piece_num = 0; piece_num < static_cast<int>(field.getElementaryPieces().size()); ++piece_num) {
        std::vector<Evaluate> ev;

        logger->info("next step start");

        evaluateNextState(state,ev);

        logger->info("evaluating field process has finished");

        makeNextState(state,ev);

        logger->info("making field process has finished");

        std::cout << "now" << (piece_num + 1) << "/" << field.getElementaryPieces().size() - field.getPieces().size() << std::endl;
        std::cout << "evaluated state size:" << ev.size() << std::endl;
        std::cout << "field size:" << state.size() << std::endl;

        //vectorのメモリ解放って頭悪くね？
        std::vector<Evaluate>().swap(ev);

//        for(auto& f : state){
//            delete_deplicate_point(f);
//        }

        bool flag = false;
        for(auto const& _field : state){
            dock->addAnswer(_field);
            if(!flag){
                submitAnswer(_field);
                flag = true;
            }
        }

        if(flag){
            last_fields.clear();
            std::copy(state.begin(),state.end(),std::back_inserter(last_fields));
        }
//        if(piece_num == 4){
//            break;
//        }
    }

    end = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    logger->warn("elapsed time: "+ std::to_string(time));

    //    neo = std::make_shared<NeoAnswerDock>();
    //    neo->show();
    //    for(const auto& f : this->debug_field){
    //        neo->addAnswer(f);
    //    }
    //    test();

//    if(!last_fields.empty()){
//        if(last_fields.at(0).getPieces().size() != last_fields.at(0).getElementaryPieces().size()){
//            TryNextSearch *next = new TryNextSearch();
//            next->setField(last_fields.at(0));
//            next->show();

//            connect(next,&TryNextSearch::startBeamSearch,[&](procon::NeoField next_field){
//                this->tryNextBeamSearch(next_field);
//            });
//        }
//    }
}

void NeoBeamSearch::tryNextBeamSearch(procon::NeoField next_field)
{
    std::cout << "called next beamsearch slot" << std::endl;

    //時間計測
    std::chrono::system_clock::time_point start,end;
    start = std::chrono::system_clock::now();

    std::vector<procon::NeoField> state;

    dock->addAnswer(next_field);

    state.push_back(next_field);

//    ev.resize(2000000);
    for (int piece_num = 0; piece_num < static_cast<int>(next_field.getElementaryPieces().size() - next_field.getPieces().size()); ++piece_num) {
        std::vector<Evaluate> ev;

        logger->info("next step start");

        evaluateNextState(state,ev);

        logger->info("evaluating field process has finished");

        makeNextState(state,ev);

        logger->info("making field process has finished");

        std::cout << "now" << (piece_num + 1) << "/" << next_field.getElementaryPieces().size() << std::endl;
        std::cout << "evaluated state size:" << ev.size() << std::endl;
        std::cout << "field size:" << state.size() << std::endl;

        //vectorのメモリ解放って頭悪くね？
        std::vector<Evaluate>().swap(ev);

        bool flag = false;
        for(auto const& _field : state){

            if(this->answer_progress_enabled){
                neo->addAnswer(_field);
            }

            if(!flag){
                submitAnswer(_field);
                flag = true;
            }
        }

        if(flag){
            last_fields.clear();
            std::copy(state.begin(),state.end(),std::back_inserter(last_fields));
        }
//        if(piece_num == 4){
//            break;
//        }
    }

    if(!last_fields.empty()){
        if(last_fields.at(0).getPieces().size() != last_fields.at(0).getElementaryPieces().size()){
//            TryNextSearch *next = new TryNextSearch();
//            next->setField(last_fields.at(0));
//            next->show();

//            connect(next,&TryNextSearch::startBeamSearch,[&](procon::NeoField next_field){
//                this->tryNextBeamSearch(next_field);
//            });

            last_selector->show();

            for(auto& f : last_fields){
                last_selector->addAnswer(f);
            }

            last_selector->show();
        }
    }else{
        logger->warn("FIELD OR PUZZULE DATA IS INVALID");
    }

    end = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    logger->warn("elapsed time: "+ std::to_string(time));
}

