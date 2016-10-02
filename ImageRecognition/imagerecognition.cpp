#include "imagerecognition.h"

procon::Field ImageRecognition::run(cv::Mat raw_frame_image, cv::Mat raw_pieces_image)
{
    //raw_pieces_pic = cv::Mat(raw_pieces_image,cv::Rect(250,0,1450,1080));
    raw_pieces_pic = raw_pieces_image;

    //前処理
    cv::Mat frame_image = preprocessingFrame(raw_frame_image);
    std::vector<cv::Mat> pieces_images = preprocessingPieces(raw_pieces_image);

    std::vector<cv::Mat> images;
    images.push_back(frame_image);
    for(cv::Mat& piece : pieces_images) images.push_back(piece);

    //線分検出
    std::vector<std::vector<cv::Vec4f>> pieces_lines = LineDetection(images);

    //ベクター化
    std::vector<polygon_t> polygons = Vectored(pieces_lines);

    return makeField(polygons);
}

void ImageRecognition::threshold(cv::Mat& image)
{
    //resize
    image = cv::Mat(image,cv::Rect(0,500,2664,3300));

    /*
    cv::namedWindow("capture",cv::WINDOW_NORMAL);
    cv::imshow("capture",image);
    */


    /* kido
    // get d
    std::vector<cv::Mat> white_channels(3);
    cv::Mat hsv_white;
    cv::cvtColor(white, hsv_white, CV_BGR2HSV);
    cv::split(hsv_white, white_channels);
    cv::Mat whiteD = white_channels[2];
    int rows = white.rows;
    int cols = white.cols;

    std::vector<cv::Mat> image_channels(3);
    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, CV_BGR2HSV);
    cv::split(hsv_image, image_channels);
    cv::Mat imageD = image_channels[2];

    // smooth
    int smooth_threshold = 0;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
        int av=0;
        int av_cnt=0;
        for (int dy = -smooth_threshold; dy < smooth_threshold; dy++) for (int dx = -smooth_threshold; dx <= smooth_threshold; dx++){
            int ny = y + dy;
            int nx = x + dx;
            if(0 <= ny && ny < rows && 0 <= nx && nx < cols){
                av_cnt++;
                av += whiteD.at<unsigned char>(ny,nx);
            }
        }
        whiteD.at<unsigned char>(y,x) = av/av_cnt;
    }

    // ave
    unsigned long int Dsum=0;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
        Dsum += whiteD.at<unsigned char>(y,x);
    }
    int ave = Dsum/(rows*cols);
    std::cout<<"ave="<<ave<<std::endl;

    // fit
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
        imageD.at<unsigned char>(y,x) += ave - whiteD.at<unsigned char>(y,x);
    }


    // merge d
    image_channels[2] = imageD;
    cv::merge(image_channels, hsv_image);
    cv::cvtColor(hsv_image, image, CV_HSV2BGR);

    */

    cv::Mat normal_area,koge_area;

    //色抽出 H:0-180/180, S:76-255/255, B:76-153/255
    colorExtraction(&image, &normal_area, CV_BGR2HSV, 0, 180, 89, 255, 76, 148);
    //colorExtraction(&image, &normal_area, CV_BGR2HSV, 0, 180, 89, 255, 76, 140);
    colorExtraction(&image, &koge_area, CV_BGR2HSV, 5, 20, 153, 255, 43, 90);

    //グレースケールに変換
    cvtColor(normal_area,normal_area,CV_RGB2GRAY);
    cvtColor(koge_area,koge_area,CV_RGB2GRAY);

    //二値化
    cv::threshold(normal_area,normal_area, 0, 255, cv::THRESH_BINARY_INV);
    cv::threshold(koge_area,koge_area, 0, 255, cv::THRESH_BINARY_INV);

    //syn
    cv::bitwise_and(normal_area,koge_area,image);

    /*
    cv::namedWindow("capturerer",cv::WINDOW_NORMAL);
    cv::imshow("capturerer",image);
    cv::namedWindow("capturer",cv::WINDOW_NORMAL);
    cv::imshow("capturer",normal_area);
    cv::namedWindow("capturern",cv::WINDOW_NORMAL);
    cv::imshow("capturern",koge_area);
    */
}

cv::Mat ImageRecognition::preprocessingFrame(cv::Mat image)
{
    threshold(image);
    int rows = image.rows;
    int cols = image.cols;

    //ピース内に混じっている白い穴を削除
    cv::Mat hole_label;
    cv::Mat hole_label_stats;
    cv::Mat buf;
    int hole_num = cv::connectedComponentsWithStats(image, hole_label, hole_label_stats, buf, 4);
    std::vector<int> labels_area;
    for(int i=2;i<hole_num;++i){
        labels_area.push_back(hole_label_stats.ptr<int>(i)[cv::ConnectedComponentsTypes::CC_STAT_AREA]);
    }
    int max_val=0;
    int max_num=0;
    for(int i=0;i<(int)labels_area.size();++i){
        if(labels_area[i] > max_val){
            max_val = labels_area[i];
            max_num = i + 2;
        }
    }
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = (hole_label.at<int>(y,x) == 1 || hole_label.at<int>(y,x) == max_num) ? 255 : 0;

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピースを一つ一つに分ける
    struct minmax2D{
        int minX = 10000;
        int maxX = 0;
        int minY = 10000;
        int maxY = 0;
    };
    std::vector<cv::Mat> images;
    cv::Mat *piece_label = new cv::Mat();
    int label_num = cv::connectedComponents(image, *piece_label);
    std::vector<struct minmax2D> minmaxs(label_num);
    for(int i=0;i<label_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
        for(int i=0;i<label_num;++i){
            images[i].at<unsigned char>(y,x) = 255;
        }
    }
    int n;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        n = piece_label->at<int>(y,x);
        if(n!=0){
            images[n-1].at<unsigned char>(y,x) = 0;
            if(minmaxs[n-1].minX > x) minmaxs[n-1].minX = x;
            if(minmaxs[n-1].maxX < x) minmaxs[n-1].maxX = x;
            if(minmaxs[n-1].minY > y) minmaxs[n-1].minY = y;
            if(minmaxs[n-1].maxY < y) minmaxs[n-1].maxY = y;
        }
    }
    delete piece_label;

    //triming, and delete small noise
    const int NOIZE_SIZE = 100;
    std::vector<cv::Mat> result_images;
    int count=0;
    for(auto &im: images){
        if(minmaxs[count].maxX-minmaxs[count].minX < NOIZE_SIZE && minmaxs[count].maxY-minmaxs[count].minY < NOIZE_SIZE){
            count++;
            continue;
        }
        result_images.push_back(cv::Mat(im,cv::Rect(minmaxs[count].minX - 5 < 0 ? 0 : minmaxs[count].minX - 5,
                                                    minmaxs[count].minY - 5 < 0 ? 0 : minmaxs[count].minY - 5,
                                                    minmaxs[count].maxX + 5 > cols ? cols-minmaxs[count].minX : minmaxs[count].maxX-minmaxs[count].minX + 10,
                                                    minmaxs[count].maxY + 5 > rows ? rows-minmaxs[count].minY : minmaxs[count].maxY-minmaxs[count].minY + 10
                                                    )));
        count++;
    }
    return std::move(result_images[0]);
}

std::vector<cv::Mat> ImageRecognition::preprocessingPieces(cv::Mat image)
{
    threshold(image);
    int rows = image.rows;
    int cols = image.cols;

    //ピース内に混じっている白い穴を削除
    cv::Mat hole_label;
    cv::connectedComponents(image, hole_label,4);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = hole_label.at<int>(y,x) == 1  ? 255 : 0;

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピースを一つ一つに分ける

    //get blackzone label
    cv::Mat blackzone_label;
    cv::Mat blackzone_stats;
    cv::Mat blackzone_centroids;
    int blackzone_num = cv::connectedComponentsWithStats(image, blackzone_label, blackzone_stats, blackzone_centroids, 4);

    //delete small noise from image
    const int NOIZE_SIZE = 100;
    std::vector<bool> isErase;
    for(int i=0;i<blackzone_num;++i){
        if(i==0){
            //first label is white zone.
            isErase.push_back(false);
        }else{
            isErase.push_back(blackzone_stats.ptr<int>(i)[cv::ConnectedComponentsTypes::CC_STAT_AREA] < NOIZE_SIZE ? true : false);
        }
    }
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        if(isErase.at(blackzone_label.at<int>(y,x))){
            image.at<unsigned char>(y,x) = 0;
        }
    }

    //get pieces label
    cv::Mat pieces_label;
    cv::Mat pieces_stats;
    cv::Mat pieces_centroids;
    int piece_num = cv::connectedComponentsWithStats(image, pieces_label, pieces_stats, pieces_centroids, 4);
    piece_num--; //white zone

    //make random color
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0,255);
    for(int i=0;i<piece_num;++i){
        raw_random_colors.push_back(cv::Vec3b(rand(rd),rand(rd),rand(rd)));
    }

    //reset images
    std::vector<cv::Mat> images;
    for(int i=0;i<piece_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    for(int i=0;i<piece_num;++i){
        for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
            images.at(i).at<unsigned char>(y,x) = 255;
        }
    }

    //sepalate image, and calc minmax, and make color image
    struct minmax2D{
        int minX = 10000;
        int maxX = 0;
        int minY = 10000;
        int maxY = 0;
    };
    std::vector<struct minmax2D> minmaxs(piece_num);
    cv::Mat color_image(rows,cols,CV_8UC3);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        int n = pieces_label.at<int>(y,x);
        if(n!=0){
            //separate image
            images[n-1].at<unsigned char>(y,x) = 0;
            //calc minmax
            if(minmaxs[n-1].minX > x) minmaxs[n-1].minX = x;
            if(minmaxs[n-1].maxX < x) minmaxs[n-1].maxX = x;
            if(minmaxs[n-1].minY > y) minmaxs[n-1].minY = y;
            if(minmaxs[n-1].maxY < y) minmaxs[n-1].maxY = y;
        }
        //make color image
        color_image.at<cv::Vec3b>(y,x) = (n==0) ? cv::Vec3b(255,255,255) : raw_random_colors[n-1];
    }
    raw_colored_pic = color_image;

    //triming, and get piece raw pos
    std::vector<cv::Mat> result_images;
    int count=0;
    for(auto &im: images){
        result_images.push_back(cv::Mat(im,cv::Rect(minmaxs[count].minX - 5 < 0 ? 0 : minmaxs[count].minX - 5,
                                                    minmaxs[count].minY - 5 < 0 ? 0 : minmaxs[count].minY - 5,
                                                    minmaxs[count].maxX + 5 > cols ? cols-minmaxs[count].minX : minmaxs[count].maxX-minmaxs[count].minX + 10,
                                                    minmaxs[count].maxY + 5 > rows ? rows-minmaxs[count].minY : minmaxs[count].maxY-minmaxs[count].minY + 10
                                                    )));
        raw_pieces_pos.push_back({minmaxs[count].minX+((minmaxs[count].maxX-minmaxs[count].minX)/2),
                                  minmaxs[count].minY+((minmaxs[count].maxY-minmaxs[count].minY)/2)});
        count++;
    }
    return std::move(result_images);
}

std::vector<std::vector<cv::Vec4f>> ImageRecognition::LineDetection(std::vector<cv::Mat> const& images)
{
    std::vector<std::vector<cv::Vec4f>> pieces_lines;

    int count = 0;
    for(auto &image : images){
        pieces_lines.push_back(std::vector<cv::Vec4f>());

        //LSD直線検出 引数の"scale"が重要！！！
        cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD,0.40);
        lsd->detect(image, pieces_lines[count]);

        //描画
        cv::Mat pic(image);
        lsd->drawSegments(pic, pieces_lines[count]);
        //if (count + 1 == 14) {
        //    cv::namedWindow(std::to_string(count+1),CV_WINDOW_NORMAL);
        //    cv::imshow(std::to_string(count+1), pic);
        //}
        count++;
    }

    return std::move(pieces_lines);
}

std::vector<polygon_t> ImageRecognition::Vectored(std::vector<std::vector<cv::Vec4f>> const& pieces_lines)
{
    /****二点の距離を返す関数****/
    auto calcDistance = [](double x1,double y1,double x2,double y2)->double{
        return std::sqrt(std::pow(x1 - x2,2) + std::pow(y1 - y2,2));
    };
    /****ここまで****/

    /***線分の順番を適切に並び替える関数***/
    auto sortLines = [&](std::vector<cv::Vec4f> lines)->std::vector<cv::Vec4f> {

        //ある線分の終点に対して他の線分の始点を調べ近い順に入れ替える
        //距離を評価値とした選択ソート
        for (int i = 0;i < static_cast<int>(lines.size() - 1); i++){

            const double end_x = lines.at(i)[2];
            const double end_y = lines.at(i)[3];

            double min = 4000;
            double min_subscript = i + 1;

            for (int j = i + 1; j < static_cast<int>(lines.size()); j++){

                const double begin_x = lines.at(j)[0];
                const double begin_y = lines.at(j)[1];

                const double distance = calcDistance(begin_x,begin_y,end_x,end_y);

                if (min > distance){
                    min = distance;
                    min_subscript = j;
                }
            }
            std::swap(lines.at(i + 1),lines.at(min_subscript));
        }
        return std::move(lines);
    };

    /***切れてる線分を修復する関数***/
    auto repairLines = [](std::vector<cv::Vec4f> lines)->std::vector<cv::Vec4f> {

        /****x軸に対する角度を計算****/
        auto calcAngle = [](double x1,double y1,double x2,double y2)->double{
            const double angle = std::atan2(std::abs(y2 - y1),std::abs(x2 - x1));
            const double slope = (y2 - y1) / (x2 - x1);
            return slope >= 0 ? angle : -angle;
        };
        /****ここまで****/

        //返り値をのせるやつ
        std::vector<cv::Vec4f> ret_lines;

        //連続して線が切れている回数
        int connection_failure_count = 0;
        const int lines_size = static_cast<int>(lines.size());

        for (int i = 0;i < lines_size;i++) {

            //1本め(現在)の線分の始点(x1,y1)，終点(x2,y2)
            double x1,x2,y1,y2;
            x1 = lines.at(i % lines_size)[0];
            y1 = lines.at(i % lines_size)[1];
            x2 = lines.at(i % lines_size)[2];
            y2 = lines.at(i % lines_size)[3];

            //2本めの線分に当たる添字を失敗回数から算出
            int next_i = (i + 1 + connection_failure_count) % lines_size;

            //2本め(次)の線分の始点(x3,y3)，終点(x4,y4)
            const double x3 = lines.at(next_i)[0];
            const double y3 = lines.at(next_i)[1];
            const double x4 = lines.at(next_i)[2];
            const double y4 = lines.at(next_i)[3];

            //それぞれの線分のx軸に対する角度
            const double angle1 = calcAngle(x1,y1,x2,y2);
            const double angle2 = calcAngle(x3,y3,x4,y4);

            //許容角度
            //(3.141592 / 180)でdeg -> radに
            constexpr double allowable_angle = 1 * (3.141592 / 180);

            //二つの線分の角度の差が許容角度以下ならば次の線分を更新する
            if (std::abs(angle1-angle2) < allowable_angle) {
                connection_failure_count++;
                i--;
            } else {
                double last_x,last_y;
                if (next_i == 0) {
                    last_x = lines.at(lines_size - 1)[2];
                    last_y = lines.at(lines_size - 1)[3];
                } else {
                    last_x = lines.at(next_i - 1)[2];
                    last_y = lines.at(next_i - 1)[3];
                }
                cv::Vec4f tmp(x1,y1,last_x,last_y);
                ret_lines.push_back(tmp);
                i += connection_failure_count;
                connection_failure_count = 0;
                if (i >= lines_size) {
                    ret_lines.erase(ret_lines.begin());
                }
            }
        }
        return ret_lines;
    };
    /***ここまで***/

    auto convertLineToPolygon = [](std::vector<cv::Vec4f> const& lines)->polygon_t {

        /****2線分の交点をだす関数****/
        auto calcIntersection = [](std::vector<double> a,std::vector<double> b)->double {

            //分子項
            const double numer1 = (a.at(2) - a.at(1)) * (a.at(4) - a.at(3)) * (b.at(3) - b.at(1));
            const double numer2 = (a.at(1)) * (b.at(2) - b.at(1)) * (a.at(4) - a.at(3));
            const double numer3 = (a.at(3)) * (b.at(4) - b.at(3)) * (a.at(2) - a.at(1));

            //分母項
            const double denom1 = (b.at(2) - b.at(1)) * (a.at(4) - a.at(3));
            const double denom2 = (b.at(4) - b.at(3)) * (a.at(2) - a.at(1));

            return ((numer1 + numer2 - numer3) / (denom1 - denom2));
        };
        /****ここまで****/

        polygon_t polygon;
        for (int i = -1 ;i < static_cast<int>(lines.size() - 1);i++){

            //それぞれ一本目と二本目のベクトル
            cv::Vec4f v1,v2;
            if (i == -1){
                v1 = lines.at(static_cast<int>(lines.size() - 1));
                v2 = lines.at(i + 1);
            } else {
                v1 = lines.at(i);
                v2 = lines.at(i + 1);
            }

            //1本目の始点(x1,y1),終点(x2,y2)
            const double x1 = v1[0] , y1 = v1[1];
            const double x2 = v1[2] , y2 = v1[3];

            //2本目の始点(x1,y1),終点(x2,y2)
            const double x3 = v2[0] , y3 = v2[1];
            const double x4 = v2[2] , y4 = v2[3];

            //xとyにまとめる
            const std::vector<double> vec_x{0,x1,x2,x3,x4};
            const std::vector<double> vec_y{0,y1,y2,y3,y4};

            //交点の算出
            const double x = calcIntersection(vec_x,vec_y);
            const double y = calcIntersection(vec_y,vec_x);

            polygon.outer().push_back(point_t(x,y));
        }
        polygon.outer().push_back(polygon.outer().at(0));
        return polygon;
    };
    /***ここまで***/

    /***一定の長さに満たない線分を消去***/
    auto removeShortLines = [&](std::vector<cv::Vec4f> const& lines)->std::vector<cv::Vec4f>{
        std::vector<cv::Vec4f> return_lines;
        for(auto line : lines){
            if ((calcDistance(line[0],line[1],line[2],line[3]) * scale) >= 0.5) {
                return_lines.push_back(std::move(line));
            }
        }
        return std::move(return_lines);
    };
    /***ここまで***/

    std::vector<polygon_t> polygons;
    bool frame_flag = true;
    for (std::vector<cv::Vec4f> piece_lines : pieces_lines) {
        polygon_t polygon,translated_polygon;

        if (frame_flag == true){
            double min = piece_lines.at(0)[0];
            double min_subscript = 0;
            for (int i = 0;i < static_cast<int>(piece_lines.size());i++){
                const double tmp = piece_lines.at(i)[0];
                if (tmp < min){
                    min = tmp;
                    min_subscript = i;
                }
            }
            std::swap(piece_lines.at(0),piece_lines.at(min_subscript));

        }

        piece_lines = sortLines(piece_lines);

        if (frame_flag == true){

            std::vector<std::vector<cv::Vec4f>> rings;
            std::vector<cv::Vec4f> ring;

            //謎の長さtoセンチメートル
            //scaleを頑張って測る
            //コードが最高にキモい
            const cv::Vec4f start_line = piece_lines.at(0);
            auto func = [&](cv::Vec4f line1,cv::Vec4f line2)->double {
                const double line1_distance = calcDistance(start_line[0],start_line[1],line1[2],line1[3]);
                const double line2_distance = calcDistance(start_line[0],start_line[1],line2[2],line2[3]);
                return line1_distance < line2_distance;
            };
            auto end_line = *(std::min_element(piece_lines.begin(),piece_lines.end(),func));
            double sum = 0;
            for (auto line : piece_lines) {
                sum += calcDistance(line[0],line[1],line[2],line[3]);
                if (line == end_line) break;    //キモい
            }
            scale =  30 * 4 / sum;

            polygon_t outer_polygon,inner_polygon,frame_polygon;

            for (int i = 0;i < static_cast<int>(piece_lines.size());i++){
                double distance = 0;
                if (i != 0){
                    const cv::Vec4f line1 = piece_lines.at(i - 1);
                    const cv::Vec4f line2 = piece_lines.at(i);
                    distance = calcDistance(line1[2],line1[3],line2[0],line2[1]) * scale;
                }

                //許容幅20mm(要検証)
                constexpr double weight_threshold = 2;

                if (distance > weight_threshold) {
                    rings.push_back(ring);
                    ring.clear();
                }
                ring.push_back(piece_lines.at(i));
            }
            rings.push_back(ring);

            auto tmp_rings =std::move(rings);
            rings.clear();
            for (auto& r : tmp_rings) {
                if (r.size() > 2) {
                    rings.emplace_back(std::move(r));
                }
            }

            rings.at(0) = repairLines(rings.at(0));
            frame_polygon = convertLineToPolygon(rings.at(0));

            for (int i = 1;i < static_cast<int>(rings.size());i++){
                rings.at(i) = repairLines(rings.at(i));
                rings.at(i) = removeShortLines(rings.at(i));
                inner_polygon = convertLineToPolygon(rings.at(i));
                frame_polygon.inners().push_back(polygon_t::ring_type());
                for (auto point : inner_polygon.outer()) {
                    frame_polygon.inners().back().push_back(point);
                }
            }

            polygons.push_back(frame_polygon);
            frame_flag = false;

        } else {
            piece_lines = repairLines(piece_lines);
            piece_lines = removeShortLines(piece_lines);
            polygons.push_back(convertLineToPolygon(piece_lines));
        }
    }

    //表示
    /*
    int count = 0;
    for (auto po:polygons){
        procon::ExpandedPolygon ishowta;
        ishowta.resetPolygonForce(po);
        count++;
        std::cout << "piece" << bg::dsv(po) << std::endl;
    }*/
    //std::cout << "frame" << bg::dsv(polygons.at(0)) << std::endl;
    return std::move(polygons);
}

procon::Field ImageRecognition::makeField(std::vector<polygon_t> polygons){

    std::vector<procon::ExpandedPolygon> ex_pieces;
    procon::ExpandedPolygon ex_frame;
    procon::Field field;
    bool frame_flag = true;

    int piece_count = 0;
    for (auto polygon : polygons){
        //座標移動
        double translateX = 0;
        double translateY = 0;
        if (frame_flag){
            //左上が(0,0)になるように移動
            double minX=30.0,minY=30.0;
            for(auto point : polygon.outer()){
                if(point.x() < minX) minX = point.x();
                if(point.y() < minY) minY = point.y();
            }
            translateX = minX;
            translateY = minY;
        }else{
            //中心が(0,0)になるように移動
            /*
            double sumX=0.0,sumY=0.0;
            for(auto point : polygon.outer()){
                sumX += point.x();
                sumY += point.y();
            }
            translateX = sumX / polygon.outer().size();
            translateY = sumY / polygon.outer().size();
            */
            const point_t center = bg::return_centroid<point_t>(polygon);
            translateX = center.x();
            translateY = center.y();
        }
        polygon_t translated_polygon;
        bg::strategy::transform::translate_transformer<double,2,2> translate(-translateX,-translateY);
        bg::transform(polygon,translated_polygon,translate);

        //縮小
        bg::strategy::transform::scale_transformer<double, 2, 2> reduction(scale);
        bg::transform(translated_polygon,polygon,reduction);
        bg::reverse(polygon);
        if (frame_flag){
            ex_frame.resetPolygonForce(polygon);
        } else {
            procon::ExpandedPolygon ex_polygon(piece_count);
            ex_polygon.resetPolygonForce(polygon);
            ex_pieces.push_back(ex_polygon);
            piece_count++;
        }
        if (frame_flag) frame_flag = false;
    }

    field.setElementaryPieces(ex_pieces);
    field.setElementaryFrame(ex_frame);

    return field;
}

void ImageRecognition::colorExtraction(cv::Mat* src, cv::Mat* dst,
    int code,
    int ch1Lower, int ch1Upper,
    int ch2Lower, int ch2Upper,
    int ch3Lower, int ch3Upper
    )
{
    cv::Mat colorImage;
    int lower[3];
    int upper[3];

    cv::Mat lut = cv::Mat(256, 1, CV_8UC3);

    cv::cvtColor(*src, colorImage, code);

    lower[0] = ch1Lower;
    lower[1] = ch2Lower;
    lower[2] = ch3Lower;

    upper[0] = ch1Upper;
    upper[1] = ch2Upper;
    upper[2] = ch3Upper;

    for (int i = 0; i < 256; i++){
        for (int k = 0; k < 3; k++){
            if (lower[k] <= upper[k]){
                if ((lower[k] <= i) && (i <= upper[k])){
                    lut.data[i*lut.step+k] = 255;
                }else{
                    lut.data[i*lut.step+k] = 0;
                }
            }else{
                if ((i <= upper[k]) || (lower[k] <= i)){
                    lut.data[i*lut.step+k] = 255;
                }else{
                    lut.data[i*lut.step+k] = 0;
                }
            }
        }
    }

    //LUTを使用して二値化
    cv::LUT(colorImage, lut, colorImage);

    //Channel毎に分解
    std::vector<cv::Mat> planes;
    cv::split(colorImage, planes);

    //マスクを作成
    cv::Mat maskImage;
    cv::bitwise_and(planes[0], planes[1], maskImage);
    cv::bitwise_and(maskImage, planes[2], maskImage);

    //出力
    cv::Mat maskedImage;
    src->copyTo(maskedImage, maskImage);
    *dst = maskedImage;
}
