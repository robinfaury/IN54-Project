#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "str.hpp"

using namespace cv;
using namespace std;

// Utility class for preparing the images (set to similar sizes, gray levels etc.)

class Preprocessing
{
public:
	static void ResizeByWidth(Mat& mat, int width, int interpolation = INTER_LINEAR)
	{
		double ratio = static_cast<double>(mat.size().width) / static_cast<double>(mat.size().height);
		resize(mat, mat, Size(width, static_cast<int>(width / ratio)), 0, 0, interpolation);
	}
	static void ResizeByHeight(Mat& mat, int height, int interpolation = INTER_LINEAR)
	{
		double ratio = static_cast<double>(mat.size().width) / static_cast<double>(mat.size().height);
		resize(mat, mat, Size(static_cast<int>(height * ratio), height), 0, 0, interpolation);
	}
	static void Preprocess(Mat& mat)
	{
		ResizeByWidth(mat, 1000);

		cvtColor(mat, mat, COLOR_BGR2GRAY);
	}
};

#endif