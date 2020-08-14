#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstring>
using namespace cv;
#define FRAME_CROP 4
class CameraData{
  private:
    Mat img;
    Rect bound;
    char victim;
    std::string ID;
  public:
    void read(int height, int width, void * frame);
    void display(std::string s);
};