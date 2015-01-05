#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "str.hpp"

#define M_PI 3.14159265359

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
	static Point2f RotateDegree(Point2f p1, Point2f p2, float alpha)
	{
		float a = M_PI*alpha/180.0;
		p1 = p1 - p2;
		float x = (p1.x)*cos(a) - (p1.y)*sin(a);
		float y = (p1.x)*sin(a) + (p1.y)*cos(a);
		return Point2f(x, y)+p2;
	}
	static void Preprocess(Mat& mat, int angle)
	{
		//ResizeByWidth(mat, 500);

		cvtColor(mat, mat, COLOR_BGR2GRAY);

		//equalizeHist( mat, mat );
	}
};

#endif