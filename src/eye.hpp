#ifndef EYE_HPP
#define EYE_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include "str.hpp"

using namespace cv;
using namespace std;

// Data structure used to store all the information about an eye

class Eye
{
public:
	Rect boundingBox;
	int reliabilityFactor;
public:
	Eye(Rect& rect) : boundingBox(rect), reliabilityFactor(1)
	{

	}
	~Eye()
	{

	}
};

typedef list<Eye> Eyes;

#endif