#ifndef GRAY_IMAGE_FFT_H
#define GRAY_IMAGE_FFT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat circShift(const cv::Mat& mat, int x, int y);
cv::Mat psf2otf(const cv::Mat& mat, cv::Size outSize = cv::Size(-1, -1));

cv::Mat grayImageFFT(const cv::Mat& mat);
cv::Mat fft2Shift(const cv::Mat& mat);
cv::Mat grayImageIFFT(const cv::Mat& mat);

#endif