#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>
#include <vector>

#include "ImageBlur.h"
#include "ImageNoise.h"
#include "WienerFiltering.h"
#include "GrayImageFFT.h"

// I/O
cv::Mat getInput(const std::string& picPath);
void outputImage(const cv::Mat& mat, const std::string& picPath);

// Processing
std::vector<cv::Mat> processImage();
void updateNoiseImage();
void updateRestoreImage();

// Display
void displayImage(const cv::Mat& mat, const std::string& windowName);

// Trackbar event listener
void onBar1(int num, void* userInput);
void onBar2(int num, void* userInput);

// Globla Variables
int sigma10 = 30; // The sigma of Gaussian Blur times 10
int k10000 = 15; // The intensity of Gausssian Blur times 10000
int kernelSideLength = 9; // The side length of Gaussian Blur Kernel

cv::Mat originalMat, noiseMat, restoreMat; // Three images

int main()
{
	// Input
	originalMat = getInput("I_o.jpg");
	
	// Process
	auto matArr = processImage();

	// Output and display
	outputImage(matArr[0], "I_n.jpg");
	outputImage(matArr[1], "I_d.jpg");

	displayImage(originalMat, "I_o");
	displayImage(matArr[0], "I_n");
	displayImage(matArr[1], "I_d");

	// Interaction
	cv::createTrackbar("kernel side length", "I_n", &kernelSideLength, 10, onBar1);
	cv::createTrackbar("10 * sigma", "I_n", &sigma10, 100, onBar1);
	cv::createTrackbar("10000 * k", "I_d", &k10000, 100, onBar2);

	cv::waitKey();

	return 0;
}

cv::Mat getInput(const std::string& picPath)
{
	cv::Mat mat;
	cv::imread(picPath, cv::IMREAD_GRAYSCALE).convertTo(mat, CV_32F, 1 / 255.0);
	return mat;
}
void outputImage(const cv::Mat& mat, const std::string& picPath)
{
	cv::Mat tmpMat;
	mat.convertTo(tmpMat, CV_8U, 255.0f);
	cv::imwrite(picPath, tmpMat);
}

void updateNoiseImage()
{
	originalMat.copyTo(noiseMat);
	exeGaussianBlur(noiseMat, double(sigma10) / 10, kernelSideLength);
	addGaussianNoise(noiseMat, 0.005);
}

void updateRestoreImage()
{
	noiseMat.copyTo(restoreMat);
	winerFilteringGaussian(restoreMat, double(k10000) / 10000, double(sigma10) / 10, kernelSideLength);
}

std::vector<cv::Mat> processImage()
{
	std::vector<cv::Mat> res;

	updateNoiseImage();
	updateRestoreImage();

	res.push_back(noiseMat);
	res.push_back(restoreMat);
	return res;
}

void displayImage(const cv::Mat& mat, const std::string& windowName)
{
	cv::imshow(windowName, mat);
}

void onBar1(int num, void* input)
{
	updateNoiseImage();
	updateRestoreImage();
	displayImage(noiseMat, "I_n");
	displayImage(restoreMat, "I_d");
}

void onBar2(int num, void* input)
{
	updateRestoreImage();
	displayImage(restoreMat, "I_d");
}