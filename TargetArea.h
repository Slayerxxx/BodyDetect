#pragma once
#include <cv.hpp>
using namespace cv;

void getMiddleArea(Mat& src);
void RemoveSmallRegion2(Mat& src, Mat& dest, int area);
