#include "polygonconnector.h"

#include "utilities.h"
#include "polygonviewer.h"

//#define DEBUG_RING

PolygonConnector::PolygonConnector()
{

}

//ピースならouterを、フレームなら指定のinner(反転する)を返す（最後の点は消す）
Ring PolygonConnector::popRingByPolygon(procon::ExpandedPolygon& polygon, int inner_position)
{
    if(inner_position == -1){
        Ring ring = polygon.getPolygon().outer();
        ring.pop_back();
        return ring;
    }else{
        Ring inner = polygon.getPolygon().inners().at(inner_position);
        Ring outer;
        int inner_size = inner.size();
        for(int i=0; i < inner_size-1; ++i){ //not copy last
            outer.push_back(inner[i]);
        }
        return outer;
    }
}

//ピースならouterを、フレームなら指定のinner(反転させる)とringを置き換える（最後の点を追加する）
polygon_t PolygonConnector::pushRingToPolygonT(Ring& ring, procon::ExpandedPolygon const& polygon, int inner_position)
{
    ring.push_back(*ring.begin());

    polygon_t new_raw_polygon = polygon.getPolygon();

    if(inner_position == -1){
        new_raw_polygon.outer().clear();
        for(auto point : ring){
            new_raw_polygon.outer().push_back(point);
        }
    }else{
        Ring inner;
        int ring_size = ring.size();
        for(int i=0; i < ring_size; ++i){
            inner.push_back(ring[i]);
        }
        new_raw_polygon.inners().at(inner_position).clear();
        for(auto point : inner){
            new_raw_polygon.inners().at(inner_position).push_back(point);
        }
    }

    return new_raw_polygon;
}

//ポリゴンを合体する関数本体 !!!!!!polygon2 mast piece
bool PolygonConnector::joinPolygon(procon::ExpandedPolygon jointed_polygon, procon::ExpandedPolygon piece, procon::ExpandedPolygon& new_polygon, std::array<Fit,2> join_data)
{
#ifdef DEBUG_RING
    auto debugRing = [](Ring ring, int line){
        std::cout<<std::to_string(line)<<" : ";
        for (int i=0; i < static_cast<int>(ring.size()); i++) {
            double x = ring[i].x();
            double y = ring[i].y();
            std::cout<<"<"<<x<<","<<y<<">";
        }
        std::cout<<std::endl;
    };
#endif

    //結合情報
    Fit fit1 = join_data[0];
    Fit fit2 = join_data[1];

    //それぞれOuterとして持つ
    Ring ring1 = popRingByPolygon(jointed_polygon, jointed_polygon.getInnerSize() == 0 ? -1 : fit1.frame_inner_pos);
    Ring ring2 = popRingByPolygon(piece, -1);
    int size1 = ring1.size();
    int size2 = ring2.size();

#ifdef DEBUG_RING
    debugRing(ring1,__LINE__);
    debugRing(ring2,__LINE__);
#endif

    //結合後に座標が一致する始点及び終点を取得
    const int complete_matching_start_pos_1 = fit1.start_dot_or_line == Fit::Dot ? fit1.start_id : fit1.start_id                  ;
    const int complete_matching_end_pos_1   = fit1.end_dot_or_line   == Fit::Dot ? fit1.end_id   : increment(fit1.end_id, size1)  ;
    const int complete_matching_start_pos_2 = fit2.start_dot_or_line == Fit::Dot ? fit2.start_id : increment(fit2.start_id, size2);
    const int complete_matching_end_pos_2   = fit2.end_dot_or_line   == Fit::Dot ? fit2.end_id   : fit2.end_id                    ;

    // 回転　Ring2を回転させる。
    bool first_radian = true;
    double criteria_radian;
    double result_radian = 0;

    double rotate_radian_average = 0.0;
    int rotate_radian_count = 0;
    if(fit1.start_dot_or_line == Fit::Dot){
        const double x1 = ring1[Utilities::dec(complete_matching_start_pos_1,size1)].x() - ring1[complete_matching_start_pos_1].x();
        const double y1 = ring1[Utilities::dec(complete_matching_start_pos_1,size1)].y() - ring1[complete_matching_start_pos_1].y();
        const double x2 = ring2[Utilities::inc(complete_matching_start_pos_2,size2)].x() - ring2[complete_matching_start_pos_2].x();
        const double y2 = ring2[Utilities::inc(complete_matching_start_pos_2,size2)].y() - ring2[complete_matching_start_pos_2].y();
        const double degree2 = atan2(y2, x2);
        const double degree1 = atan2(y1, x1);
        double rotate_radian = (degree1 - degree2);
        //std::cout<<"aaa"<<rotate_radian<<std::endl;
        if(first_radian){
            first_radian = false;
            criteria_radian = rotate_radian;
            //rotate_radian_average += rotate_radian;
        }

        //rotate_radian_count++;
        //std::cout<<"ccc"<<rotate_radian<<std::endl;
    }
    do{
        const double x1 = ring1[Utilities::inc(complete_matching_start_pos_1,size1,rotate_radian_count)].x() - ring1[Utilities::inc(complete_matching_start_pos_1,size1,1 + rotate_radian_count)].x();
        const double y1 = ring1[Utilities::inc(complete_matching_start_pos_1,size1,rotate_radian_count)].y() - ring1[Utilities::inc(complete_matching_start_pos_1,size1,1 + rotate_radian_count)].y();
        const double x2 = ring2[Utilities::dec(complete_matching_start_pos_2,size2,rotate_radian_count)].x() - ring2[Utilities::dec(complete_matching_start_pos_2,size2,1 + rotate_radian_count)].x();
        const double y2 = ring2[Utilities::dec(complete_matching_start_pos_2,size2,rotate_radian_count)].y() - ring2[Utilities::dec(complete_matching_start_pos_2,size2,1 + rotate_radian_count)].y();
        const double degree2 = atan2(y2, x2);
        const double degree1 = atan2(y1, x1);
        double rotate_radian = (degree1 - degree2);

        //std::cout<<"v"<<rotate_radian<<std::endl;
        double distance_radian = 0;
        if(first_radian){
            first_radian = false;
            criteria_radian = rotate_radian;
            //rotate_radian_average += rotate_radian;
        }else{
            if(rotate_radian < criteria_radian){
                rotate_radian += M_PI * 2;
            }
            distance_radian = rotate_radian - criteria_radian;
            if(distance_radian >= M_PI){
                distance_radian -= M_PI * 2;
            }
            rotate_radian_average += distance_radian;
        }

        rotate_radian_count++;
        //std::cout<<"d"<<rotate_radian_count<<","<<rotate_radian<<","<<distance_radian<<std::endl;
    }while((Utilities::inc(complete_matching_start_pos_1,size1,rotate_radian_count-1) != complete_matching_end_pos_1) &&
           (Utilities::inc(complete_matching_start_pos_1,size1,rotate_radian_count) != complete_matching_end_pos_1));
    //rotate_radian_average /= (double)(fit1.start_dot_or_line == Fit::Dot? 1 + rotate_radian_count : rotate_radian_count);
    if(rotate_radian_count != 1){
        result_radian = rotate_radian_average / (double)(fit1.start_dot_or_line == Fit::Dot? rotate_radian_count : rotate_radian_count - 1);
    }
    result_radian = criteria_radian + result_radian;
    //std::cout<<"result:"<<result_radian<<std::endl;

    piece.rotatePolygon(-result_radian*(360/(M_PI*2))); //rotate piece
    ring2 = popRingByPolygon(piece,-1); //update ring2

    //debugRing(ring1,__LINE__);
    //debugRing(ring2,__LINE__);

    // 移動　結合後に一致する点とその次の点を用いて、ポリゴンのx,y移動を調べ、Polygon2を平行移動
    const int Join_point1 = complete_matching_start_pos_1;
    const int Join_point2 = complete_matching_start_pos_2;
    const double move_x = ring1[Join_point1].x() - ring2[Join_point2].x();
    const double move_y = ring1[Join_point1].y() - ring2[Join_point2].y();
    piece.translatePolygon(move_x, move_y); //translate piece
    ring2 = popRingByPolygon(piece,-1); //update ring2

    // 重複チェック！
    if(hasConflict(ring1, ring2, fit1, fit2)){
        return false;
    }

#ifdef DEBUG_RING
    debugRing(ring1,__LINE__);
    debugRing(ring2,__LINE__);
#endif

    // 結合　新しいRingに結合後の外周の角を入れる。
    // もし、結合端の辺の長さが等しくならない時はRing1,Ring2ともに端の角を入力。
    // ここで回転の誤差により角が一致しない場合がある。
    Ring new_ring;
    int count = complete_matching_end_pos_1 + 1;
    int Type = 1;
    double x,y;

    // Frame Area
    while(1){
        x = ring1[count%size1].x();
        y = ring1[count%size1].y();

        if (count % size1 == complete_matching_start_pos_1){
            break;
        }

        new_ring.push_back(point_t(x,y));
        count++;
    }

    // Frame & Piece Area
    bool start_pos_flag = true;
    while(1){
        if(start_pos_flag){
            start_pos_flag = false;
            // Switch Area
            if (fit1.start_dot_or_line == Fit::Dot) { //dot_or_lineはどちらのポリゴンでも同じですね…仕様が変だ
                count = complete_matching_start_pos_2 + 1;
            } else {
                count = complete_matching_start_pos_2;
            }
            // Straight Check
            if(fit1.start_dot_or_line == Fit::Line && fit1.is_start_straight == true){
                // Line is straight. Skip.
                count++;
                x = ring2[count%size2].x();
                y = ring2[count%size2].y();
            }else{
                if (fit1.start_dot_or_line == Fit::Dot) {
                    x = ring2[count%size2].x();
                    y = ring2[count%size2].y();
                }else{
                    // calc average
                    x = (ring2[count%size2].x() + ring1[complete_matching_start_pos_1].x()) / 2.0;
                    y = (ring2[count%size2].y() + ring1[complete_matching_start_pos_1].y()) / 2.0;
                }
            }
        }else{
            x = ring2[count%size2].x();
            y = ring2[count%size2].y();
        }

        if (count % size2 == (fit2.end_dot_or_line == Fit::Dot ? (((complete_matching_end_pos_2 - 1) % size2 + size2) % size2) : complete_matching_end_pos_2)) {
            break;
        }

        new_ring.push_back(point_t(x,y));
        count++;
    }
    if(fit1.end_dot_or_line == Fit::Line && fit1.is_end_straight == true){
        // Line is straight. Skip.
    }else{
        if (fit1.end_dot_or_line == Fit::Dot) {
        }else{
            // calc average
            x = (ring2[complete_matching_end_pos_2].x() + ring1[complete_matching_end_pos_1].x()) / 2.0;
            y = (ring2[complete_matching_end_pos_2].y() + ring1[complete_matching_end_pos_1].y()) / 2.0;
        }
        new_ring.push_back(point_t(x,y));
    }

#ifdef DEBUG_RING
    debugRing(new_ring,__LINE__);
#endif

    //　ポリゴンにRingを出力しておしまい
    //TODO
    if(jointed_polygon.getInnerSize() != 0){ //frame-piece
        polygon_t new_raw_polygon = pushRingToPolygonT(new_ring, jointed_polygon, fit1.frame_inner_pos);
        jointed_polygon.setMultiIds(std::vector<int>{jointed_polygon.getId(), piece.getId()});
        new_polygon = std::move(jointed_polygon);
        new_polygon.pushNewJointedPolygon(new_raw_polygon, piece);
    }else{ //piece-piece
        throw "Not supported!";
        new_polygon.setMultiIds(std::vector<int>{jointed_polygon.getId(), piece.getId()});
        polygon_t new_raw_polygon = pushRingToPolygonT(new_ring, new_polygon);
        //new_polygon.pushNewJointedPolygon(jointed_polygon, join_data);
        //new_polygon.pushNewJointedPolygon(piece, join_data);
    }

    return true;
}

//重複を見つける。
bool PolygonConnector::hasConflict(Ring ring1, Ring ring2, Fit fit1, Fit fit2)
{
    int size1 = ring1.size();
    int size2 = ring2.size();
    //結合後に座標が一致する始点及び終点を取得
    const int cmstart1 = fit1.start_dot_or_line == Fit::Dot ? fit1.start_id : fit1.start_id                  ;
    const int cmend1   = fit1.end_dot_or_line   == Fit::Dot ? fit1.end_id   : increment(fit1.end_id, size1)  ;
    const int cmstart2 = fit2.start_dot_or_line == Fit::Dot ? fit2.start_id : increment(fit2.start_id, size2);
    const int cmend2   = fit2.end_dot_or_line   == Fit::Dot ? fit2.end_id   : fit2.end_id                    ;

    bool ring1_yello_start_zone = false;
    bool ring1_orange_start_zone = false;
    bool ring1_red_zone = false;
    bool ring1_orange_end_zone = true;
    bool ring1_yellow_end_zone = false;
    bool ring1_white_zone = false;

    int ring1_pos = cmend1; //orange end zone
    if((ring1_pos+2)%size1 == cmstart1 && fit1.start_dot_or_line == Fit::Dot){
        //ring1_orange_end_zone = false;
        ring1_yello_start_zone = true;
    }else if((ring1_pos+1)%size1 == cmstart1 && fit1.start_dot_or_line == Fit::Line){
        //ring1_orange_end_zone = false;
        ring1_orange_start_zone = true;
    }

    for(int i=0;i<size1;++i){

        //make ring
        point_t line1_start = ring1[ring1_pos];
        point_t line1_end = ring1[(ring1_pos+1)%size1];


        bool ring2_yello_start_zone = false;
        bool ring2_orange_start_zone = false;
        bool ring2_red_zone = false;
        bool ring2_orange_end_zone = true;
        bool ring2_yellow_end_zone = false;
        bool ring2_white_zone = false;

        int ring2_pos = cmend2; //orange end zone
        if(((ring2_pos-2)%size2+size2)%size2 == cmstart2 && fit2.start_dot_or_line == Fit::Dot){
            //ring2_orange_end_zone = false;
            ring2_yello_start_zone = true;
        }else if(((ring2_pos-1)%size2+size2)%size2 == cmstart2 && fit2.start_dot_or_line == Fit::Line){
            //ring2_orange_end_zone = false;
            ring2_orange_start_zone = true;
        }

        for(int j=0;j<size2;++j){

            //skip check
            if(     ring1_white_zone ||
                    (ring1_yellow_end_zone && !ring2_orange_end_zone && (ring2_yellow_end_zone || ring2_white_zone || ring2_red_zone || ring2_orange_start_zone || ring2_yello_start_zone)) ||
                    (ring1_orange_end_zone && !ring2_red_zone && !ring2_yellow_end_zone && !ring2_orange_end_zone && (ring2_white_zone || (ring2_orange_start_zone && (cmstart1!=cmend1 || fit2.start_dot_or_line == Fit::Line)) || ring2_yello_start_zone)) ||
                    (ring1_red_zone && !ring2_red_zone && !ring2_orange_end_zone && !ring2_orange_start_zone && (ring2_white_zone || ring2_yellow_end_zone || ring2_yellow_end_zone)) ||
                    (ring1_orange_start_zone && !ring2_red_zone && !ring2_orange_start_zone && !ring2_yello_start_zone && (ring2_white_zone || ring2_yellow_end_zone || (ring2_orange_end_zone && (cmstart1 != cmend1 || fit2.start_dot_or_line == Fit::Line)))) ||
                    (ring1_yello_start_zone && !ring2_orange_start_zone && (ring2_yello_start_zone || ring2_white_zone || ring2_yellow_end_zone || ring2_orange_end_zone || ring2_red_zone))
              ){
                //make ring
                point_t line2_start = ring2[ring2_pos];
                point_t line2_end = ring2[((ring2_pos - 1)%size2+size2)%size2];

                //check conflict
                if(static_cast<bool>(Utilities::cross_check(line1_start, line1_end, line2_start, line2_end))){
                    return true;
                }
            }

            //dec
            ring2_pos--;
            if(ring2_pos == -1) ring2_pos = size2 - 1;

            //toggle
            if(ring2_orange_end_zone){
                ring2_orange_end_zone = false;
                if(fit2.end_dot_or_line == Fit::Dot){
                    ring2_yellow_end_zone = true;
                }else{
                    ring2_white_zone = true;
                }
            }else if(ring2_yellow_end_zone && (fit2.start_dot_or_line == Fit::Dot ? ring2_yello_start_zone == false : ring2_orange_start_zone == false)){
                ring2_yellow_end_zone = false;
                ring2_white_zone = true;
            }
            if(((ring2_pos-2)%size2+size2)%size2 == cmstart2 && fit2.start_dot_or_line == Fit::Dot){
                ring2_white_zone = false;
                ring2_yello_start_zone = true;
            }else if(((ring2_pos-1)%size2+size2)%size2 == cmstart2 && fit2.start_dot_or_line == Fit::Line){
                ring2_white_zone = false;
                ring2_orange_start_zone = true;
            }else if(ring2_yello_start_zone){
                ring2_yellow_end_zone = false;
                ring2_yello_start_zone = false;
                ring2_orange_start_zone = true;
            }else if(ring2_orange_start_zone){
                ring2_yellow_end_zone = false;
                ring2_orange_start_zone = false;
                ring2_red_zone = true;
            }
        }
        //inc
        ring1_pos++;
        if(ring1_pos == size1) ring1_pos = 0;

        //toggle
        if(ring1_orange_end_zone){
            ring1_orange_end_zone = false;
            if(fit1.end_dot_or_line == Fit::Dot){
                ring1_yellow_end_zone = true;
            }else{
                ring1_white_zone = true;
            }
        }else if(ring1_yellow_end_zone && (fit1.start_dot_or_line == Fit::Dot ? ring1_yello_start_zone == false : ring1_orange_start_zone == false)){
            ring1_yellow_end_zone = false;
            ring1_white_zone = true;
        }
        if((ring1_pos+2)%size1 == cmstart1 && fit1.start_dot_or_line == Fit::Dot){
            ring1_white_zone = false;
            ring1_yello_start_zone = true;
        }else if((ring1_pos+1)%size1 == cmstart1 && fit1.start_dot_or_line == Fit::Line){
            ring1_white_zone = false;
            ring1_orange_start_zone = true;
        }else if(ring1_yello_start_zone){
            ring1_yellow_end_zone = false;
            ring1_yello_start_zone = false;
            ring1_orange_start_zone = true;
        }else if(ring1_orange_start_zone){
            ring1_yellow_end_zone = false;
            ring1_orange_start_zone = false;
            ring1_red_zone = true;
        }
    }
    return false;
}
