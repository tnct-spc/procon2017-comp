#include "imagerecognition.h"

procon::Field ImageRecognition::run(std::string const& path)
{
    //std::string path = "./../../procon2016-comp/picture/scan.png";

    //前処理
    std::vector<cv::Mat> images = Preprocessing(path);

    //線分検出
    std::vector<std::vector<cv::Vec4f>> pieces_lines = LineDetection(images);

    //ベクター化
    std::vector<polygon_t> polygons = Vectored(pieces_lines);

    //cv::waitKey();
    return makeField(polygons);
}

std::vector<cv::Mat> ImageRecognition::Preprocessing(std::string const& path)
{
    //画像読み込み
    cv::Mat image = cv::imread(path, 1);
    int rows = image.rows;
    int cols = image.cols;

    //色抽出 H:0-255/180, S:51-255/255, B:133-230/255
    colorExtraction(&image, &image, CV_BGR2HSV, 0, 180, 51, 255, 133, 191);

    //グレースケールに変換
    cvtColor(image,image,CV_RGB2GRAY);

    //二値化
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY_INV);

    //ピース内に混じっている白い穴を削除
    cv::Mat *hole_label = new cv::Mat();
    cv::connectedComponents(image, *hole_label,4);
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) image.at<unsigned char>(y,x) = hole_label->at<int>(y,x) == 1  ? 255 : 0;
    delete hole_label;

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
    std::vector<struct minmax2D> minmaxs(label_num-1);
    for(int i=1;i<label_num;++i) images.push_back(cv::Mat(rows,cols,CV_8UC1));
    int n;
    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++)
    {
        n = piece_label->at<int>(y,x);
        if(n!=0){
            images[n-1].at<unsigned char>(y,x) = 255;
            if(minmaxs[n-1].minX > x) minmaxs[n-1].minX = x;
            if(minmaxs[n-1].maxX < x) minmaxs[n-1].maxX = x;
            if(minmaxs[n-1].minY > y) minmaxs[n-1].minY = y;
            if(minmaxs[n-1].maxY < y) minmaxs[n-1].maxY = y;
        }
    }
    delete piece_label;

    //triming and delete small noise
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

    //二値化
    for(auto &im : images){
        cv::threshold(im, im, 0, 255, cv::THRESH_BINARY_INV);
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
        cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(cv::LSD_REFINE_STD,0.35,0.6,2.0,22.5,0,0.7,1024);
        lsd->detect(image, pieces_lines[count]);

        //描画

        cv::Mat pic(image);
        lsd->drawSegments(pic, pieces_lines[count]);
        cv::namedWindow(std::to_string(count+1),CV_WINDOW_NORMAL);
        cv::imshow(std::to_string(count+1), pic);

        count++;
    }

    return std::move(pieces_lines);
}

std::vector<polygon_t> ImageRecognition::Vectored(std::vector<std::vector<cv::Vec4f>> const& pieces_lines)
{
    auto sortLines = [](std::vector<cv::Vec4f> lines)->std::vector<cv::Vec4f> {
        auto calcDistance = [](double x1,double y1,double x2,double y2)->double{
            return std::sqrt(std::pow(x1 - x2,2) + std::pow(y1 - y2,2));
        };
        for (int i = 0;i < static_cast<int>(lines.size() - 1); i++){
            //ある線分の終点に対して他の線分の始点を調べ近い順に入れ替える
            const double end_x = lines.at(i)[2];
            const double end_y = lines.at(i)[3];
            double min = 4000; //dekai
            double min_subscript = 0;
            for (int j = i + 1; j < static_cast<int>(lines.size()); j++){
                const double begin_x = lines.at(j)[2];
                const double begin_y = lines.at(j)[3];
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
    std::vector<polygon_t> polygons;
    for (std::vector<cv::Vec4f> piece_lines : pieces_lines) {
        polygon_t polygon,translated_polygon;
        piece_lines = sortLines(piece_lines);
        for (int i = -1 , connection_failure_count = 0;i < static_cast<int>(piece_lines.size() - 1);i++){
            cv::Vec4f v1,v2;
            if (i == -1){
                v1 = piece_lines.at(static_cast<int>(piece_lines.size() - 1));
                v2 = piece_lines.at(i + connection_failure_count + 1);
            } else {
                v1 = piece_lines.at(i);
                v2 = piece_lines.at(i + connection_failure_count + 1);
            }

            //1本目
            const double x1 = v1[0] , y1 = v1[1];
            const double x2 = v1[2] , y2 = v1[3];
            //2本目
            const double x3 = v2[0] , y3 = v2[1];
            const double x4 = v2[2] , y4 = v2[3];
            //xとyにまとめる
            const std::vector<double> vec_x{0,x1,x2,x3,x4};
            const std::vector<double> vec_y{0,y1,y2,y3,y4};
            //表示

            //2線分の交点をだす関数
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
            //ここまで

            //二直線のx軸との角度
            const double angle1 = std::atan2(std::abs(y2 - y1),std::abs(x2 - x1));
            const double angle2 = std::atan2(std::abs(y4 - y3),std::abs(x4 - x3));
            //許容角度
            constexpr double allowable_angle = 5 * (3.14 / 180); //deg->rad
            std::cout << "angle1:" << angle1 << "angle2:" << angle2 << std::endl;
            //二つの直線の許容角度が一定以下であれば切れてる直線であると考える
            if (std::abs(angle1 - angle2) < allowable_angle) {
                connection_failure_count++;
                i--;
                continue;
            } else {
                connection_failure_count = 0;
            }
            const double x = calcIntersection(vec_x,vec_y);
            const double y = calcIntersection(vec_y,vec_x);
            polygon.outer().push_back(point_t(x,y));
        }
        polygon.outer().push_back(polygon.outer().at(0));
        std::cout << bg::dsv(polygon) << std::endl;
        polygons.push_back(polygon);
    }
    int count = 0;
    for (auto po:polygons){
        procon::ExpandedPolygon ishowta;
        ishowta.setPolygon(po);
        disp.push_back(new SinglePolygonDisplay());
        disp.at(count)->setPolygon(ishowta,4000,std::to_string(count + 1));
        disp.at(count)->show();
        count++;
    }
    return std::move(polygons);
}

procon::Field ImageRecognition::makeField(std::vector<polygon_t> polygons){
    std::vector<procon::ExpandedPolygon> ex_pieces;
    procon::ExpandedPolygon ex_flame;
    procon::Field field;
    for (auto polygon : polygons){
        polygon_t translated_polygon;
        bg::strategy::transform::translate_transformer<double,2,2> translate(-polygon.outer().at(0).x(),-polygon.outer().at(0).y());
        bg::transform(polygon,translated_polygon,translate);
        procon::ExpandedPolygon ex_polygon;
        ex_polygon.setPolygon(translated_polygon);
        ex_pieces.push_back(ex_polygon);
    }
    field.setElementaryPieces(ex_pieces);
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
