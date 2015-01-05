#ifndef FACE_HPP
#define FACE_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include "str.hpp"
#include "orientation.hpp"
#include "eye.hpp"

using namespace cv;
using namespace std;

// Data structure used to store all the information about one recognized face

class Face
{
public:
	Rect boundingBox;
	Orientation orientation;
	Eyes eyes;
	int reliabilityFactor;
	int alphaMin;
	int alphaMax;
public:
	Face(Rect& rect, int alpha) : boundingBox(rect), reliabilityFactor(1), alphaMin(alpha), alphaMax(alpha)
	{
		
	}
	~Face()
	{

	}
};

typedef list<Face> Faces;

#endif