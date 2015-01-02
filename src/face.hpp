#ifndef FACE_HPP
#define FACE_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <list>
#include "str.hpp"
#include "orientation.hpp"

using namespace cv;
using namespace std;

// Data structure used to store all the information about one recognized face

class Face
{
public:
	Rect boundingBox;
	Orientation orientation;
	int reliabilityFactor;
public:
	Face(Rect& rect) : boundingBox(rect), reliabilityFactor(1)
	{

	}
	~Face()
	{

	}
};

typedef list<Face> Faces;

#endif