#ifndef COMPONENTS_FACECV_HPP
#define COMPONENTS_FACECV_HPP

#include <iostream>
#include <thread>
#include <vector>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace devtest
{
namespace components
{

/**
 * @brief Recognizes faces in the image.
 * 
 * If at least one face is recognized in the image 
 * and the @p blurFaces flag is true - blurs areas with faces 
 * and saves a half-sized copy.
 * 
 * @param path Path to image
 * @param faceCascade Recognition cascade object
 * @param blurFaces Flag changing image with found faces
 * @return std::vector< cv::Rect > A set of areas in which faces are recognized.
 */
std::vector<cv::Rect> faceCV(
    const std::string &path, 
    cv::CascadeClassifier &faceCascade,
    bool blurFaces = true);

}

} // namespace devtest

#endif