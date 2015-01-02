#ifndef ORIENTATION_DETECTOR2_HPP
#define ORIENTATION_DETECTOR2_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"
#include "orientationdetector.hpp"

using namespace cv;
using namespace std;

// Contains the algorithm for computing the orientation of a face from its already known characteristics

class OrientationDetector2 : public OrientationDetector
{
public:
	OrientationDetector2() : OrientationDetector()
	{
	}
	virtual ~OrientationDetector2()
	{
	}
	virtual void Train()
	{

	}
	virtual void DetectOrientation(const Mat& mat, Face& face)
	{

	}
};

#endif