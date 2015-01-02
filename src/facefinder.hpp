#ifndef FACEFINDER_HPP
#define FACEFINDER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"
#include "imgparser.hpp"

using namespace cv;
using namespace std;

// Contains face recognition algorithm

class FaceFinder : public ImgParser
{
public:
	void Train()
	{
		
	}
	void FindFaces(const Mat& mat, Faces& faces)
	{
		faces.push_back(Face(Point(255, 300), 50));
		faces.push_back(Face(Point(500, 158), 40));
	}
};

#endif