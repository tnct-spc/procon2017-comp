#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H


class ImageRecognition : public ImageRecognition
{
private:
    ImageRecognition() = default;
    ~ImageRecognition() = default;
public:
    static ImageRecognition& getInstance(){
        static ImageRecognition instance;
        return instance;
    }
};

#endif // IMAGERECOGNITION_H
