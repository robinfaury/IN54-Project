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
	static void Preprocess(Mat& mat)
	{
		Mat src = mat;
		Mat dst;

		double ratio = static_cast<double>(src.size().width) / static_cast<double>(src.size().height);
		int wantedSize = 1000;

		resize(mat, dst, Size(wantedSize, static_cast<int>(wantedSize / ratio)));
		mat = dst;

		cvtColor(dst, mat, COLOR_BGR2GRAY);
	}
};

#endif