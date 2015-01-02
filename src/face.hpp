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
	Point center;
	int radius;
	Orientation orientation;
public:
	Face(Point middle, int halfDiameter) : center(middle), radius(halfDiameter)
	{

	}
	~Face()
	{

	}
};

typedef list<Face> Faces;

#endif