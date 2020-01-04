#ifndef IMAGE_BLUR_H
#define IMAGE_BLUR_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat getGaussianBlurKernal(double sigma = 1.5, unsigned sideLength = 3, cv::Point anchor = { -1, -1 });
void exeGaussianBlur(cv::Mat& mat, double sigma = 1.5, unsigned sideLength = 3);

#endif