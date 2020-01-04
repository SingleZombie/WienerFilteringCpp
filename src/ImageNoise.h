#ifndef IMAGE_NOISE_H
#define IMAGE_NOISE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Add Gaussian noise to a image
// Assume the type of mat is <Vec3f>
void addGaussianNoise(cv::Mat& mat, double k, double mu = 0.0, double sigma = 1.0);

#endif