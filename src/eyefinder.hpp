#ifndef EYEFINDER_HPP
#define EYEFINDER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "face.hpp"
#include "imgparser.hpp"
#include "preprocessing.hpp"

using namespace cv;
using namespace std;

// Contains eye recognition algorithm

class EyeFinder : public ImgParser
{
private:
	vector<CascadeClassifier> eyeClassifiers;
public:
	void Train()
	{
		eyeClassifiers.push_back(CascadeClassifier());
		eyeClassifiers.back().load("data/haarcascade_eye.xml");
	}
	void FindEyes(const Mat& mat, Face& face)
	{
		Mat src = mat(face.boundingBox);

		vector<Rect> objects;
		for (vector<CascadeClassifier>::iterator eyeClassifier = eyeClassifiers.begin(); eyeClassifier != eyeClassifiers.end(); ++eyeClassifier)
		{
			eyeClassifier->detectMultiScale(src, objects, 1.1, 4);
			for (vector<Rect>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				if (it->width > 1) // Minimum square size, else it's false positive or too far
				{
					Eyes::iterator intersectionFound = face.eyes.end();
					for (Eyes::iterator eye = face.eyes.begin(); eye != face.eyes.end(); ++eye)
					{
						Rect intersectionArea = (*it) & eye->boundingBox;
						if (intersectionArea.width != 0)
							intersectionFound = eye;
					}
					if (intersectionFound == face.eyes.end())
						face.eyes.push_back(*it);
					else
						intersectionFound->reliabilityFactor += 1;
				}
			}
			objects.clear();
		}
	}
};

#endif