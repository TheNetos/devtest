#include "gtest/gtest.h"
#include "faceCV/faceCV.hpp"

using namespace devtest::components;
using namespace std;

TEST(faceCV, recognition)
{
    cv::CascadeClassifier cas;
    ASSERT_TRUE(cas.load("haarcascade_frontalface_alt2.xml"));
    vector<cv::Rect> faces = faceCV("./testassets/1/1.jpg", cas, false);
    ASSERT_EQ(faces.size(), 1);
    faces.clear();
    faces = faceCV("./testassets/2/2.jpg", cas, false);
    ASSERT_EQ(faces.size(), 2);
    faces.clear();
    faces = faceCV("./testassets/3+/multiple.jpg", cas, false);
    ASSERT_GE(faces.size(), 3);
    faces.clear();
    faces = faceCV("./testassets/nofaces/0.jpg", cas, false);
    ASSERT_EQ(faces.size(), 0);
}

