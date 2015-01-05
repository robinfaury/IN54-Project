#ifndef EYEFINDER_HPP
#define EYEFINDER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "face.hpp"
#include "imgparser.hpp"
#include "preprocessing.hpp"
#include "eyeclassifier.hpp"

using namespace cv;
using namespace std;

// Contains eye recognition algorithm

class EyeFinder : public ImgParser
{
private:
	vector<EyeClassifier> eyeClassifiers;
public:
	void Train()
	{
		eyeClassifiers.push_back(EyeClassifier(1));
		eyeClassifiers.push_back(EyeClassifier(2));
		eyeClassifiers.push_back(EyeClassifier(3));
		eyeClassifiers.push_back(EyeClassifier(4));
		eyeClassifiers.push_back(EyeClassifier(5));
		eyeClassifiers.push_back(EyeClassifier(6));
		eyeClassifiers.push_back(EyeClassifier(7));

		for (vector<EyeClassifier>::iterator eyeClassifier = eyeClassifiers.begin(); eyeClassifier != eyeClassifiers.end(); ++eyeClassifier)
			eyeClassifier->Train();
	}
	void FindEyes(const Mat& mat, Face& face)
	{
		if (face.boundingBox.x < 0 || face.boundingBox.y < 0 || face.boundingBox.width < 20 || face.boundingBox.height < 20)
			return;
		Mat matFace = mat(face.boundingBox);

		vector<Rect> objects;
		for (vector<EyeClassifier>::iterator eyeClassifier = eyeClassifiers.begin(); eyeClassifier != eyeClassifiers.end(); ++eyeClassifier)
		{
			eyeClassifier->FindEyes(matFace, objects);
			for (vector<Rect>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				if (it->width > 1) // Minimum square size, else it's false positive or too far
				{
					it->x += face.boundingBox.x;
					it->y += face.boundingBox.y;
					Eyes::iterator intersectionFound = face.eyes.end();
					for (Eyes::iterator eye = face.eyes.begin(); eye != face.eyes.end(); ++eye)
					{
						Rect intersectionArea = (*it) & eye->boundingBox;
						if (intersectionArea.width > 0 && intersectionArea.height > 0)
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

		//for (Eyes::iterator eye = face.eyes.begin(); eye != face.eyes.end(); ++eye)
			//rectangle(matFace, eye->boundingBox, Scalar(255, 0, 0, 1), eye->reliabilityFactor/8+1, 8, 0);
		//imshow("matFace", matFace);

		//waitKey(0);
	}
};

#endif