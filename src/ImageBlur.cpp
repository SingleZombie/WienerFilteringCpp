#include "ImageBlur.h"
#include "GrayImageFFT.h"
#include <cassert>
#include <cmath>
#include <random>
#include <opencv2/highgui/highgui.hpp>  
#include <algorithm>
#include <complex>
static double standardNormalFunc2D(double x, double y, double sigma)
{
	return exp(-(x * x + y * y) / (2 * sigma * sigma)) / CV_2PI / sigma / sigma;
}

cv::Mat getGaussianBlurKernal(double sigma, unsigned sideLength, cv::Point anchor)
{
	// Initialize a kernel
	cv::Mat kernel(sideLength, sideLength, CV_32F);

	// If anchor is default then set it to the middle eof kernel
	if (anchor.x == -1 || anchor.y == -1)
	{
		anchor.x = anchor.y = sideLength / 2;
	}

	// For each entry of kernel
	for (int i = 0; i < sideLength; i++)
	{
		for (int j = 0; j < sideLength; j++)
		{
			// Get the coordinate when the origin is anchor
			int y = (i - anchor.y);
			int x = (j - anchor.x);

			// Calculate the weight with the coordinate
			kernel.at<float>(i, j) = standardNormalFunc2D(x, y, sigma);
		}
	}
	// normalization
	cv::normalize(kernel, kernel, 1, 0, cv::NORM_L1);

	return kernel;
}

void exeGaussianBlur(cv::Mat& mat, double sigma, unsigned sideLength)
{
	// Get frequent field of origin image
	cv::Mat fMat = grayImageFFT(mat);

	// Get frequent field of blur function and rearrange it
	cv::Mat h = getGaussianBlurKernal(sigma, sideLength);
	cv::Mat funcH = psf2otf(h, mat.size());
	
	// Execute complex number multiply
	for (int i = 0; i < fMat.rows; i++)
	{
		for (int j = 0; j < fMat.cols; j++)
		{
			std::complex<double> g(fMat.at<cv::Vec2f>(i, j)[0], fMat.at<cv::Vec2f>(i, j)[1]);
			std::complex<double> h(funcH.at<cv::Vec2f>(i, j)[0], funcH.at<cv::Vec2f>(i, j)[1]);

			g *= h;

			fMat.at<cv::Vec2f>(i, j)[0] = g.real();
			fMat.at<cv::Vec2f>(i, j)[1] = g.imag();
		}
	}

	// Do IFFT
	cv::Mat matRes = grayImageIFFT(fMat);
	mat = cv::Mat(matRes, cv::Rect(0, 0, mat.cols, mat.rows));
}