﻿#include "imagerecognition.h"
#include "threshold.h"
#include "polygonviewer.h"
#include "neopolygonviewer.h"
#include "utilities.h"
#include "neosinglepolygondisplay.h"

procon::NeoField ImageRecognition::run(cv::Mat raw_frame_image, cv::Mat raw_pieces_image)
{
    raw_pieces_pic = raw_pieces_image;

    // 二値化,前処理.
    //cv::Mat frame_image = preprocessingFrame(raw_frame_image); // 入れ替え可
    //std::vector<cv::Mat> pieces_images = preprocessingPieces(raw_pieces_image);

    // ピースに分割
    std::vector<cv::Mat> frame_image = dividePiece(raw_frame_image);
    std::vector<cv::Mat> pieces_images = dividePiece(raw_pieces_image);

    /*
    for (unsigned int i = 0; i < pieces_images.size(); i++) {
        cv::namedWindow(std::to_string(i+1));
        cv::imshow(std::to_string(i+3),pieces_images[i]);
    }
    */

    // つなげてるだけ
    // ピースはひとつひとつの画像に分ける
    std::vector<cv::Mat> images;
    for(cv::Mat& piece : frame_image) images.push_back(piece);
    for(cv::Mat& piece : pieces_images) images.push_back(piece);

    // 線分検出
    std::vector<std::vector<cv::Vec4f>> pieces_lines = LineDetection(images);
    //std::copy(lines.begin(), lines.end(), std::back_inserter(pieces_lines));

    // ベクター化
    std::vector<polygon_t> polygons = Vectored(pieces_lines);

    // frameのinnersをouterに入れ替える
    field_num = polygons[0].inners().size();
    for (unsigned int i=0; i<field_num; i++) {
        polygon_t inside;
        for (unsigned int j=0; j<polygons[0].inners()[i].size(); j++) {
            inside.outer().push_back(polygons[0].inners()[i].at(j));
        }
        polygons.push_back(inside);
    }

    // 元のフレームは削除
    polygons.erase(polygons.begin());

    std::vector<polygon_i> pieces;
    double error;
    for (unsigned int i=0; i<polygons.size(); i++) {
        pieces.push_back(placeGrid(polygons[i]));
        //error = getError(pieces[i], i+1);
    }

    error = getError(pieces);

    // fieldクラスのデータに変換
    // 色々変更しなきゃいけないかも
    procon::NeoField field = makeNeoField(pieces);

    return field;
}

void ImageRecognition::threshold(cv::Mat& image)
{
    //resize
    //int cols = image.cols;
    //int rows = image.rows;
    //500,0,3300,2664
    image = cv::Mat(image,cv::Rect(0,0,3300,2664));

    cv::namedWindow("capture",cv::WINDOW_NORMAL);
    cv::imshow("capture",image);

    /* ヒストグラム均一化の残骸
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

    // 色抽出 H:180, S:255, B:255
    colorExtraction(&image, &normal_area, CV_BGR2HSV, 0, 180, 89, 255, 76, 200);
    colorExtraction(&image, &koge_area, CV_BGR2HSV, 5, 20, 153, 255, 43, 90);

    cv::namedWindow("image",  CV_WINDOW_NORMAL);
    cv::imshow("image", normal_area);

    // 通常部分とこげ部分をマージ
    cv::bitwise_and(normal_area,koge_area,image);

    //cv::namedWindow("capturerer",cv::WINDOW_NORMAL);
    //cv::imshow("capturerer",image);
    //cv::namedWindow("capturer",cv::WINDOW_NORMAL);
    //cv::imshow("capturer",normal_area);
    //cv::namedWindow("capturern",cv::WINDOW_NORMAL);
    //cv::imshow("capturern",koge_area);
}

cv::Mat ImageRecognition::preprocessingFrame(cv::Mat image)
{
    // 二値化
    threshold(image);

    // 白黒を反転？
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    int rows = image.rows;
    int cols = image.cols;

    // ピースを一つ一つに分ける
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

    // ピースのサイズに画像をカット、小さいものはノイズとみなして除去
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
    // 二値化
    threshold(image);

    int rows = image.rows;
    int cols = image.cols;

    // Labeling ピース内に混じっている白い穴を削除
    cv::Mat hole_label;
    cv::connectedComponents(image, hole_label,4);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = hole_label.at<int>(y,x) == 1  ? 255 : 0;

    // 白黒を反転？
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);


    /*このあたりはつらいが綺麗にする気力がない*/


    // Delete small noise from image
    cv::Mat blackzone_label;
    cv::Mat blackzone_stats;
    cv::Mat blackzone_centroids;
    int blackzone_num = cv::connectedComponentsWithStats(image, blackzone_label, blackzone_stats, blackzone_centroids, 4);
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

    // Labeling
    cv::Mat pieces_label;
    cv::Mat pieces_stats;
    cv::Mat pieces_centroids;
    int piece_num = cv::connectedComponentsWithStats(image, pieces_label, pieces_stats, pieces_centroids, 4);
    piece_num--; //white zone

    // Make random color
    std::random_device rd;
    std::uniform_real_distribution<double> rand(0,255);
    for(int i=0;i<piece_num;++i){
        raw_random_colors.push_back(cv::Vec3b(rand(rd),rand(rd),rand(rd)));
    }

    // Reset images?
    std::vector<cv::Mat> images;
    for(int i=0;i<piece_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    for(int i=0;i<piece_num;++i){
        for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++){
            images.at(i).at<unsigned char>(y,x) = 255;
        }
    }

    // Sepalate image and make color image
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

    // Triming, and get piece raw pos
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
        cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD,threshold::LSDthrehold);
        lsd->detect(image, pieces_lines[count]);

        //描画
        cv::Mat pic(image);
        lsd->drawSegments(pic, pieces_lines[count]);
        // Debug
        //if (count < 10) {
        //    cv::namedWindow(std::to_string(count+1));
        //    cv::imshow(std::to_string(count+1), pic);
        //}
        count++;
    }

    //cv::waitKey(0);

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

            double min = 114514;
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
            constexpr double allowable_angle = 3 * (3.141592 / 180);

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

    auto convertLineToPolygon = [&](std::vector<cv::Vec4f> const& lines)->polygon_t {

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

            //平均のx,y
            const double mean_x = ((x2 + x3) / 2);
            const double mean_y = ((y2 + y3) / 2);
            //平均点と算術点の許容誤差
            //ありえん誤差
            constexpr double length_error = 3;
            if (!Utilities::nearlyEqual(mean_x,x,length_error / scale) || !Utilities::nearlyEqual(mean_y,y,length_error / scale)) {
                //auto hoge = length_error / scale;
                std::cerr << "gosaaaaaaaaaaaaaaaaaaa" << std::endl;
                polygon.outer().push_back(point_t(mean_x,mean_y));
            } else {
                polygon.outer().push_back(point_t(x,y));
            }
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

        auto tmp = std::move(sortLines(piece_lines));
        piece_lines.clear();
        piece_lines = std::move(tmp);

        if (frame_flag == true){
            {
                //謎の長さtoセンチメートル→ミリメートル
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
                scale = (210.0 * 2 + 297.0 * 2) / sum; // まわりの実際の長さ/計算上の合計？　変更
            }

            std::vector<std::vector<cv::Vec4f>> rings;
            std::vector<cv::Vec4f> ring;

            polygon_t outer_polygon,inner_polygon,frame_polygon;

            for (int i = 0;i < static_cast<int>(piece_lines.size());i++){
                double distance = 0;
                if (i != 0){
                    const cv::Vec4f line1 = piece_lines.at(i - 1);
                    const cv::Vec4f line2 = piece_lines.at(i);
                    distance = calcDistance(line1[2],line1[3],line2[0],line2[1]) * scale;
                }

                //許容幅20mm(要検証)
                constexpr double weight_threshold = 20;

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
        } else {
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

        /* Arien
        auto repairCutting = [&](polygon_t & polygon)
        {
            constexpr double cutting_allowance = 0.1;
            auto enlargePoint = [&](point_t & point){
                auto isMinus= [&](double const& a)->double
                {
                    return a < 0 ? -1 : 1;
                };
                const double x = isMinus(point.x()) * cutting_allowance + point.x();
                const double y = isMinus(point.y()) * cutting_allowance + point.y();
                point.set<0>(x);
                point.set<1>(y);
            };
            for (auto& inner : polygon.inners()) {
                for (auto& point : inner) {
                    enlargePoint(point);
                }
            }
            for (auto &point : polygon.outer()) {
                enlargePoint(point);
            }
        };repairCutting(polygon);
        */

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

void ImageRecognition::colorExtraction(cv::Mat* src, cv::Mat* dst, int code, int ch1Lower, int ch1Upper, int ch2Lower, int ch2Upper, int ch3Lower, int ch3Upper)
{
    cv::Mat colorImage;
    std::vector<int> lower = {ch1Lower,ch2Lower,ch3Lower};
    std::vector<int> upper = {ch1Upper,ch2Upper,ch3Upper};

    cv::cvtColor(*src, colorImage, code);
    cv::namedWindow("image",  CV_WINDOW_NORMAL);
    cv::imshow("image", colorImage);


    colorImage.forEach<cv::Vec3b>([&lower, &upper](cv::Vec3b &p, const int*) -> void {
        bool is_extract = [&]()->bool{
            for(int i=0;i<3;++i){
                if(!((lower[i] <= p[i]) && (p[i] <= upper[i]))) return false;
            }
            return true;
        }();

        if(is_extract){
            for(int i=0;i<3;++i) p[i] = 255;
        }else{
            for(int i=0;i<3;++i) p[i] = 0;
        }
    });

    // グレースケールに変換
    cvtColor(colorImage,colorImage,CV_RGB2GRAY);

    // 二値化
    cv::threshold(colorImage,colorImage, 0, 255, cv::THRESH_BINARY_INV);

    *dst = colorImage;
}

// RGBからエッジ検出
std::vector<cv::Vec4f> ImageRecognition::houghLine(cv::Mat src_image)
{
    // エッジ画像を検出
    cv::Mat canny_image;
    cv::Canny(src_image, canny_image, 60.0, 180.0, 3);

    // Hough変換
    std::vector<cv::Vec4f> lines;
    cv::HoughLinesP(canny_image, lines, 1, CV_PI / 180, 80, 30, 70);

    // 重複している線を削除
    auto i = lines.begin();
    while (i < lines.end()) {
        cv::Vec4f i_vec = *i;
        float slop = (i_vec[3] - i_vec[1]) / (i_vec[2] - i_vec[0]);
        for (auto j = i+1; j<lines.end(); ++j) {
            cv::Vec4f j_vec = *j;
            if (fabs(slop * (j_vec[0] - i_vec[0]) + i_vec[1] - j_vec[1]) < 70) {
                if (fabs(slop * (j_vec[2] - i_vec[0]) + i_vec[1] - j_vec[3]) < 70) {
                    i_vec = i_vec + j_vec;
                    slop = (i_vec[3] - i_vec[1]) / (i_vec[2] - i_vec[0]);
                    lines.erase(j);
                }
            }
        }
        ++i;
    }

    // 画像上に見つけた直線を設置
    cv::Vec4i pt;
    cv::Mat line_image = src_image.clone();
    int color = 100;
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        pt = *it;
        cv::line(line_image, cv::Point(pt[0], pt[1]), cv::Point(pt[2], pt[3]),color, 5, CV_AA);
        color += 10;
    }

    //cv::namedWindow("Line");
    //cv::imshow("Line",line_image);

    //cv::waitKey(100000);

    return lines;
}

// HSVから2値化
cv::Mat ImageRecognition::HSVDetection(cv::Mat src_image)
{
    // RGB画像をHSVに変換
    cv::Mat hsv_image;
    cv::cvtColor(src_image, hsv_image, CV_BGR2HSV);

    // HSVに画像を分解
    cv::Mat channels[3];
    cv::split(hsv_image, channels);

    // 色相と彩度から色抽出
    int width = hsv_image.cols;
    int height = hsv_image.rows;
    cv::Mat piece_image = cv::Mat(cv::Size(width, height), CV_8UC1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int h = channels[0].at<uchar>(y, x);
            int s = channels[1].at<uchar>(y, x);
            if ((h > 0 && h < 50) && s > 90) { // 調節
                piece_image.at<uchar>(y, x) = 255;
            }
            else {
                piece_image.at<uchar>(y, x) = 0;
            }
        }
    }

    return piece_image;
}

// 画像を分ける
std::vector<cv::Mat> ImageRecognition::dividePiece(cv::Mat src_image)
{
    int width = src_image.cols;
    int height = src_image.rows;

    // ぼかし（ノイズ削減）
    cv::GaussianBlur(src_image, src_image, cv::Size(5,5), 0);

    // 2値化
    cv::Mat bainary_image = HSVDetection(src_image);

    // ラベリング
    cv::Mat label_Image(cv::Size(width, height), CV_32S);
    cv::Mat stats;
    cv::Mat center;
    int pieces_num = cv::connectedComponentsWithStats(bainary_image, label_Image, stats, center, 4, CV_32S);

    // ノイズの除去＆分割
    std::vector<cv::Mat> pieces_images;
    for (int i = 1; i < pieces_num; i++) {

        int *param = stats.ptr<int>(i);

        // 面積の小さいものは省く
        int piece_area = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
        if (piece_area > 10000) { // 調節

            // 各ピースごとに移し替える
            cv::Mat piece_image(cv::Size(width, height), CV_8UC1);
            piece_image = cv::Scalar(255);
            for (int y = 0; y < height; y++) {
                int *lb = label_Image.ptr<int>(y);
                for (int x = 0; x < width; x++) {
                    unsigned char *pix = piece_image.ptr<unsigned char>(y);
                    if (lb[x] == i) {
                        pix[x] = 0;
                    }
                }
            }

            area.push_back(piece_area);
            pieces_images.push_back(piece_image);
        }
    }

    return pieces_images;
}

polygon_i ImageRecognition::placeGrid(polygon_t vertex)
{    
    auto polygon = vertex.outer();

    unsigned int size = polygon.size();

    /*
    // 最も長い辺から始める（誤差を減らす）
    // つけない方がいいかも…　大きいピースで誤差が出る
    double long_len = 0;
    unsigned int longgest;
    for (unsigned int i = 0; i < size-1; i++) {
        double x = vertex.outer().at(i+1).x() - vertex.outer().at(i).x();
        double y = vertex.outer().at(i+1).y() - vertex.outer().at(i).y();
        double it_len = pow(x, 2.0) + pow(y, 2.0);
        if (long_len < it_len) {
            long_len = it_len;
            longgest = i;
        }
    }

    // 配列を循環
    vertex.outer().pop_back();
    polygon_t polygon;
    for (unsigned int i = 0; i < size-1; i++) {
        polygon.outer().push_back(vertex.outer().at((i+longgest)%(size-1)));
    }
    polygon.outer().push_back(polygon.outer().at(0));
    */

    // グリッドの点番号で保存
    polygon_i grid_piece;
    grid_piece.outer().push_back(point_i(0,0));

    // 最初の辺の長さをグリッド基準で計算
    double first_x = polygon.at(1).x() - polygon.at(0).x();
    double first_y = polygon.at(1).y() - polygon.at(0).y();
    double x_dif = first_x * scale / 2.5;
    double y_dif = first_y * scale / 2.5;
    double len = sqrt(pow(x_dif, 2.0) + pow(y_dif, 2.0));

    double dif_min = 1.0;
    double dy;
    int p[2];

    // 45度の範囲から辺の長さの誤差がもっとも小さいものをピックアップ
    for (int x = ceil(len); x >= floor(len * sqrt(0.5)); x--) {

        if (len < x) {
            dy = 0;
        } else {
            dy = sqrt(pow(len, 2.0) - pow(x, 2.0));
        }

        // 計算上の点から上下の点で誤差を比べる
        double top_error = fabs(sqrt(pow(floor(dy), 2.0) + pow(x, 2.0)) - len);
        double bottom_error = fabs(sqrt(pow(ceil(dy), 2.0) + pow(x, 2.0)) - len);
        if (dif_min > top_error) {
            p[0] = x;
            p[1] = (int)floor(dy);
            dif_min = top_error;
        } else if (dif_min > bottom_error) {
            p[0] = x;
            p[1] = (int)ceil(dy);
            dif_min = bottom_error;
        }
    }

    grid_piece.outer().push_back(point_i(p[0],p[1]));

    // 回転角を算出
    double theta_1;
    if (first_x < 0 && first_y > 0) {
        theta_1 = atan(first_y / first_x) + M_PI;
    } else if (first_x < 0 && first_y <0) {
        theta_1 = atan(first_y / first_x) - M_PI;
    } else {
        theta_1 = atan(first_y / first_x);
    }
    double theta_2 = atan((double)grid_piece.outer().at(1).y() / (double)grid_piece.outer().at(1).x());
    double theta = theta_2 - theta_1;

    // 2つの角度から選ぶ
    if (theta_2 != 0) {
        double sec_x = polygon.at(2).x() - polygon.at(0).x();
        double sec_y = polygon.at(2).y() - polygon.at(0).y();
        double acute_x = sec_x * cos(theta) - sec_y * sin(theta);
        double acute_y = sec_x * sin(theta) + sec_y * cos(theta);
        double obtuse_x = sec_x * cos(M_PI/2-theta_2-theta_1) - sec_y * sin(M_PI/2-theta_2-theta_1);
        double obtuse_y = sec_x * sin(M_PI/2-theta_2-theta_1) + sec_y * cos(M_PI/2-theta_2-theta_1);
        double dacute_len = pow(acute_x - round(acute_x), 2.0) + pow(acute_y - round(acute_y), 2.0);
        double dobtuse_len = pow(obtuse_x - round(obtuse_x), 2.0) + pow(obtuse_y - round(obtuse_y), 2.0);

        if (dobtuse_len < dacute_len) theta = M_PI / 2 - theta_2 - theta_1;
    }
    // 全ての点を回転後のいちに移動
    polygon_t turn;
    turn.outer().push_back(point_t(0,0));
    for (unsigned int i=1; i<size; i++) {
        double x = polygon.at(i).x() - polygon.at(0).x();
        double y = polygon.at(i).y() - polygon.at(0).y();
        double move_x = x * cos(theta) - y * sin(theta);
        double move_y = x * sin(theta) + y * cos(theta);
        turn.outer().push_back(point_t(move_x,move_y));
    }

    // 全ての点の座標をグリッドに変換
    for (unsigned int i=2; i<size; i++) {
        double x = turn.outer().at(i).x() * scale / 2.5;
        double y = turn.outer().at(i).y() * scale / 2.5;
        grid_piece.outer().push_back(point_i(round(x),round(y)));
    }

    procon::NeoExpandedPolygon piece;
    piece.resetPolygonForce(grid_piece);

    NeoPolygonViewer::getInstance().displayPolygon(grid_piece, "Grid", false);

    return grid_piece;
}

/*
double ImageRecognition::getError(polygon_i p,int num)
{
    double piece_area = 0;

    for (unsigned int j=0; j<p.outer().size()-1; j++) {
        auto point1 = p.outer().at(j);
        auto point2 = p.outer().at(j+1);
        piece_area += (point1.x() - point2.x()) * (point1.y() +point2.y());
    }

    piece_area = fabs(piece_area) * 0.5 * 2.5 * 2.5;

    double scan_area = area[num] * pow(scale, 2.0);

    double error = scan_area - piece_area;

    return error;
}
*/

double ImageRecognition::getError(std::vector<polygon_i> p)
{
    double piece_area = 0;

    for (unsigned int i=1; i<p.size(); i++) {
        for (unsigned int j=0; j<p[i].outer().size()-1; j++) {
            auto point1 = p[i].outer().at(j);
            auto point2 = p[i].outer().at(j+1);
            piece_area += (point1.x() - point2.x()) * (point1.y() + point2.y());
        }
    }

    piece_area = fabs(piece_area) * 0.5 * 2.5 * 2.5;

    double scan_area = 0;

    for (unsigned int i=1; i<area.size(); i++) {
        scan_area += area[i];
    }

    scan_area = scan_area * pow(scale, 2.0);

    double error = scan_area - piece_area;

    return error;
}

procon::NeoField ImageRecognition::makeNeoField(std::vector<polygon_i> pieces)
{
    procon::NeoField field;

    std::vector<procon::NeoExpandedPolygon> neo_pieces;

    for (unsigned int i=0; i<pieces.size() - field_num; i++) {
        procon::NeoExpandedPolygon polygon;
        polygon.resetPolygonForce(pieces[i]);
        neo_pieces.push_back(polygon);

        field.setPiece(polygon);
    }

    std::vector<procon::NeoExpandedPolygon> neo_frame;

    for (unsigned int i=0; i<field_num; i++) {
        procon::NeoExpandedPolygon polygon;
        polygon.resetPolygonForce(pieces[pieces.size() - 1 - i]);
        neo_frame.push_back(polygon);
    }

    field.setFrame(neo_frame);

    return field;
}
