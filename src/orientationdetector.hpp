#ifndef ORIENTATION_DETECTOR
#define ORIENTATION_DETECTOR

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"
#include "imgparser.hpp"

using namespace cv;
using namespace std;

// Contains the algorithm for computing the orientation of a face from its already known characteristics

class OrientationDetector : public ImgParser
{
public:
	OrientationDetector()
	{
	}
	virtual ~OrientationDetector()
	{
	}
	virtual void Train() = 0;
	virtual void DetectOrientation(const Mat& mat, Face& face) = 0;
};

#endif