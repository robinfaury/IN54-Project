#ifndef ORIENTATION_DETECTOR1_HPP
#define ORIENTATION_DETECTOR1_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"
#include "orientationdetector.hpp"

using namespace cv;
using namespace std;

// Contains the algorithm for computing the orientation of a face from its already known characteristics

class OrientationDetector1 : public OrientationDetector
{
public:
	OrientationDetector1() : OrientationDetector()
	{
	}
	virtual ~OrientationDetector1()
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