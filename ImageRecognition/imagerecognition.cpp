#include "imagerecognition.h"

procon::Field ImageRecognition::run(cv::Mat raw_flame_image, cv::Mat raw_pieces_image)
{
    raw_pieces_pic = cv::Mat(raw_pieces_image,cv::Rect(250,0,1450,1080));

    //前処理
    cv::Mat flame_image = preprocessingFlame(raw_flame_image);
    std::vector<cv::Mat> pieces_images = preprocessingPieces(raw_pieces_image);

    std::vector<cv::Mat> images;
    images.push_back(flame_image);
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
    image = cv::Mat(image,cv::Rect(250,0,1450,1080));

    //色抽出 H:0-180/180, S:76-255/255, B:76-153/255
    colorExtraction(&image, &image, CV_BGR2HSV, 0, 180, 76, 255, 76, 153);

    //グレースケールに変換
    cvtColor(image,image,CV_RGB2GRAY);

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);
}

cv::Mat ImageRecognition::preprocessingFlame(cv::Mat image)
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
    const int NOIZE_SIZE = 10;
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
        cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD,0.35);
        lsd->detect(image, pieces_lines[count]);

        //描画
        cv::Mat pic(image);
        lsd->drawSegments(pic, pieces_lines[count]);
        //if (count + 1 == 14) {
            cv::namedWindow(std::to_string(count+1),CV_WINDOW_NORMAL);
            cv::imshow(std::to_string(count+1), pic);
        //}
        count++;
    }

    return std::move(pieces_lines);
}

std::vector<polygon_t> ImageRecognition::Vectored(std::vector<std::vector<cv::Vec4f>> const& pieces_lines)
{
    /***線分の順番を適切に並び替える関数***/
    auto sortLines = [](std::vector<cv::Vec4f> lines)->std::vector<cv::Vec4f> {

        /****二点の距離を返す関数****/
        auto calcDistance = [](double x1,double y1,double x2,double y2)->double{
            return std::sqrt(std::pow(x1 - x2,2) + std::pow(y1 - y2,2));
        };
        /****ここまで****/

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
    /***ここまで***/

    /***線分を修復する関数***/
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
        double connection_failure_count = 0;

        for (int i = -1;i < static_cast<int>(lines.size() - 1);i++) {

            //1本め(現在)の線分の始点(x1,y1)，終点(x2,y2)
            double x1,x2,y1,y2;
            if (i == -1) {
                x1 = lines.at(static_cast<int>(lines.size() - 1))[0];
                y1 = lines.at(static_cast<int>(lines.size() - 1))[1];
                x2 = lines.at(static_cast<int>(lines.size() - 1))[2];
                y2 = lines.at(static_cast<int>(lines.size() - 1))[3];
            } else {
                x1 = lines.at(i)[0];
                y1 = lines.at(i)[1];
                x2 = lines.at(i)[2];
                y2 = lines.at(i)[3];
            }

            //2本めの線分に当たる添字を失敗回数から算出
            int next_i = i + 1 + connection_failure_count;
            //out_of_range対策
            if (next_i >= static_cast<int>(lines.size())) {
                next_i -= static_cast<int>(lines.size());
            }

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
            constexpr double allowable_angle = 5 * (3.141592 / 180);

            //二つの線分の角度の差が許容角度以下ならば次の線分を更新する
            if (std::abs(angle1-angle2) < allowable_angle) {
                connection_failure_count++;
                i--;
            } else {
                double last_x,last_y;
                if (next_i == 0) {
                    last_x = lines.at(static_cast<int>(lines.size() - 1))[2];
                    last_y = lines.at(static_cast<int>(lines.size() - 1))[3];
                } else {
                    last_x = lines.at(next_i - 1)[2];
                    last_y = lines.at(next_i - 1)[3];
                }
                cv::Vec4f tmp(x1,y1,last_x,last_y);
                ret_lines.push_back(tmp);
                connection_failure_count = 0;
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

    std::vector<polygon_t> polygons;
    bool flame_flag = true;
    for (std::vector<cv::Vec4f> piece_lines : pieces_lines) {
        polygon_t polygon,translated_polygon;

        if (flame_flag == true){
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

        if (flame_flag == true){
            std::vector<cv::Vec4f> outer,inner;
            polygon_t outer_polygon,inner_polygon;
            for (int i = 0;i < static_cast<int>(piece_lines.size());i++){
                if (i < 4) {
                    outer.push_back(piece_lines.at(i));
                } else {
                    inner.push_back(piece_lines.at(i));
                }
            }
            outer = repairLines(outer);
            outer_polygon = convertLineToPolygon(outer);
            inner = repairLines(inner);
            inner_polygon = convertLineToPolygon(inner);

            polygons.push_back(inner_polygon);
            flame_flag = false;
            //Flame幅から全体のスケールを計算
            const double flame_length = bg::distance(outer_polygon.outer()[0],outer_polygon.outer()[1]);
            const double default_flame_length = 30;
            scale = default_flame_length / flame_length;

        } else {
            piece_lines = repairLines(piece_lines);
            polygons.push_back(convertLineToPolygon(piece_lines));
        }
    }

    //表示
    int count = 0;
    for (auto po:polygons){
        procon::ExpandedPolygon ishowta;
        ishowta.setPolygon(po);
        disp.push_back(new SinglePolygonDisplay());
        disp.at(count)->setPolygon(ishowta,4000,std::to_string(count + 1));
        disp.at(count)->show();
        count++;
    }
    std::cout << bg::dsv(polygons.at(0)) << std::endl;
    return std::move(polygons);
}

procon::Field ImageRecognition::makeField(std::vector<polygon_t> polygons){

    std::vector<procon::ExpandedPolygon> ex_pieces;
    procon::ExpandedPolygon ex_flame;
    procon::Field field;
    bool flame_flag = true;

    int piece_count = 0;
    for (auto polygon : polygons){
        //座標移動
        double translateX = 0;
        double translateY = 0;
        if (flame_flag){
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
            double sumX=0.0,sumY=0.0;
            for(auto point : polygon.outer()){
                sumX += point.x();
                sumY += point.y();
            }
            translateX = sumX / polygon.outer().size();
            translateY = sumY / polygon.outer().size();
        }
        polygon_t translated_polygon;
        bg::strategy::transform::translate_transformer<double,2,2> translate(-translateX,-translateY);
        bg::transform(polygon,translated_polygon,translate);

        //縮小
        bg::strategy::transform::scale_transformer<double, 2, 2> reduction(scale);
        bg::transform(translated_polygon,polygon,reduction);
        bg::reverse(polygon);
        std::cout << bg::area(polygon) << std::endl;
        if (flame_flag){
            ex_flame.setPolygon(polygon);
        } else {
            procon::ExpandedPolygon ex_polygon(piece_count);
            ex_polygon.setPolygon(polygon);
            ex_pieces.push_back(ex_polygon);
            piece_count++;
        }
        if (flame_flag) flame_flag = false;
    }

    field.setElementaryPieces(ex_pieces);
    field.setElementaryFlame(ex_flame);
    field.printFlame();
    field.printPiece();

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
