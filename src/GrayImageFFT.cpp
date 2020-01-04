#include "GrayImageFFT.h"

cv::Mat circShift(const cv::Mat& mat, int x, int y)
{
	int mWidth = mat.cols;
	int mHeight = mat.rows;
	x = (x % mWidth + mWidth) % mWidth;
	y = (y % mHeight + mHeight) % mHeight;

	cv::Mat res(mat.size(), mat.type());
	cv::Mat m1(mat, cv::Rect(0, 0, mWidth - x, mHeight));
	cv::Mat m2(mat, cv::Rect(mWidth - x, 0, x, mHeight));
	m1.copyTo(res(cv::Rect(x, 0, mWidth - x, mHeight)));
	m2.copyTo(res(cv::Rect(0, 0, x, mHeight)));
	
	cv::Mat res2(mat.size(), mat.type());
	m1 = cv::Mat(res, cv::Rect(0, 0, mWidth, mHeight - y));
	m2 = cv::Mat(res, cv::Rect(0, mHeight - y, mWidth, y));
	m1.copyTo(res2(cv::Rect(0, y, mWidth, mHeight - y)));
	m2.copyTo(res2(cv::Rect(0, 0, mWidth, y)));

	return res2;
}

cv::Mat psf2otf(const cv::Mat& mat, cv::Size outSize)
{
	if (outSize.height == -1 || outSize.width == -1)
	{
		outSize = mat.size();
	}

	cv::Mat otf;
	cv::Size paddleSize = outSize - mat.size();
	cv::copyMakeBorder(mat, otf,
		paddleSize.height - paddleSize.height / 2, paddleSize.height / 2,
		paddleSize.width - paddleSize.width / 2, paddleSize.width / 2,
		cv::BORDER_CONSTANT,
		cv::Scalar::all(0));

	otf = circShift(otf, -otf.size().width / 2, -otf.size().height / 2);
	otf = grayImageFFT(otf);
	
	return otf;
}

cv::Mat grayImageFFT(const cv::Mat& mat)
{
	cv::Mat res;
	cv::dft(mat, res, cv::DFT_COMPLEX_OUTPUT);

	return res;
}

cv::Mat fft2Shift(const cv::Mat& mat)
{
	cv::Mat res(mat.size(), mat.type());
	cv::Mat m1 = cv::Mat(mat, cv::Rect(0, 0, mat.cols / 2, mat.rows / 2));
	cv::Mat m2 = cv::Mat(mat, cv::Rect(mat.cols / 2, 0, mat.cols / 2, mat.rows / 2));
	cv::Mat m3 = cv::Mat(mat, cv::Rect(0, mat.rows / 2, mat.cols / 2, mat.rows / 2));
	cv::Mat m4 = cv::Mat(mat, cv::Rect(mat.cols / 2, mat.rows / 2, mat.cols / 2, mat.rows / 2));
	m1.copyTo(res(cv::Rect(mat.cols / 2, mat.rows / 2, mat.cols / 2, mat.rows / 2)));
	m2.copyTo(res(cv::Rect(0, mat.rows / 2, mat.cols / 2, mat.rows / 2)));
	m3.copyTo(res(cv::Rect(mat.cols / 2, 0, mat.cols / 2, mat.rows / 2)));
	m4.copyTo(res(cv::Rect(0, 0, mat.cols / 2, mat.rows / 2)));
	return res;
}

cv::Mat grayImageIFFT(const cv::Mat& mat)
{
	cv::Mat res;
	cv::dft(mat, res, cv::DFT_SCALE | cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
	

	return res;
}
