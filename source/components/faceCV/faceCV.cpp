#include "faceCV.hpp"

std::vector<cv::Rect> devtest::components::faceCV(
    const std::string &path,
    cv::CascadeClassifier &faceCascade,
    bool blurFaces)
{
    std::vector<cv::Rect> foundFaces;
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    if (img.data)
    {
        std::cout << "[faceCV] "
                  << "Loaded image: " << path << ", mat size: " << img.size << std::endl;
        faceCascade.detectMultiScale(img, foundFaces, 1.1, 2, 
            0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        std::cout << "[faceCV] "
                  << "Founded " << foundFaces.size() << " faces" << std::endl;
                  
        if (foundFaces.size() && blurFaces) {
            for (const cv::Rect &maskRect : foundFaces) {
                cv::GaussianBlur(img(maskRect), img(maskRect), cv::Size(0,0), 6);
            }
            cv::resize(img, img, 
                cv::Size(
                    static_cast<int>(img.size().width / 2), 
                    static_cast<int>(img.size().height / 2)
                ));
            std::string scaledImgPath = path;
            scaledImgPath.insert(scaledImgPath.find_last_of('.'), "_scaled");
            cv::imwrite(scaledImgPath, img);
        }
    }
    else
    {
        std::cout << "[faceCV] "
                  << "Could not open file: " << path << std::endl;
    }
    return foundFaces;
}