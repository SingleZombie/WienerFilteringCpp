#include "ImageNoise.h"
#include <cassert>
#include <cmath>
#include <random>
#include <algorithm>

void addGaussianNoise(cv::Mat& mat, double k, double mu, double sigma)
{
	// cpp standard normal distribution generator
	std::random_device divice;
	std::default_random_engine rng(divice());
	std::normal_distribution<> normDis(mu, sigma);

	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			// cal the result by adding noise
			float tmp = mat.at<float>(i, j) + k * normDis(rng);

			// assignment
			mat.at<float>(i, j) = tmp;
		}
	}
}