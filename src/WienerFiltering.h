#ifndef WIENER_FILTERING_H
#define WIENER_FILTERING_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void winerFilteringGaussian(cv::Mat& mat, double k, double sigma, int sideLength);

#endif