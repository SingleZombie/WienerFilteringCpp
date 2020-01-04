#include "WienerFiltering.h"
#include "GrayImageFFT.h"
#include "ImageBlur.h"
#include <complex>

using comp = std::complex<double>;

static void filtering(cv::Mat& mat, const cv::Mat& degrationFunc, double k)
{
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			comp g(mat.at<cv::Vec2f>(i, j)[0], mat.at<cv::Vec2f>(i, j)[1]);
			comp h(degrationFunc.at<cv::Vec2f>(i, j)[0], degrationFunc.at<cv::Vec2f>(i, j)[1]);
			double h2 = std::abs(h) * std::abs(h);
			comp res = h2 == 0 ? comp{0, 0} : h2 / (h2 + k) / h * g;
			mat.at<cv::Vec2f>(i, j)[0] = res.real();
			mat.at<cv::Vec2f>(i, j)[1] = res.imag();
		}
	}
}

void winerFilteringGaussian(cv::Mat& mat, double k, double sigma, int sideLength)
{
	// Get G(u, v)
	cv::Mat fMat = grayImageFFT(mat);

	// Get H(u, v)
	cv::Mat funch = getGaussianBlurKernal(sigma, sideLength);
	cv::Mat funcH = psf2otf(funch, mat.size());

	// Calculate the formula
	filtering(fMat, funcH, k);

	// Do IFFT
	cv::Mat matRes = grayImageIFFT(fMat);

	// Resize and get result
	mat = cv::Mat(matRes, cv::Rect(0, 0, mat.cols, mat.rows));
}

