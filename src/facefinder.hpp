#ifndef FACEFINDER_HPP
#define FACEFINDER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "face.hpp"
#include "imgparser.hpp"
#include "preprocessing.hpp"

using namespace cv;
using namespace std;

// Contains face recognition algorithm

class FaceFinder : public ImgParser
{
private:
	vector<CascadeClassifier> faceClassifiers;
public:
	void Train()
	{
		faceClassifiers.push_back(CascadeClassifier());
		faceClassifiers.back().load("data/haarcascade_frontalface_alt.xml");
		//faceClassifiers.push_back(CascadeClassifier());
		//faceClassifiers.back().load("data/haarcascade_eye.xml");
		//faceClassifiers.push_back(CascadeClassifier());
		//faceClassifiers.back().load("data/haarcascade_frontalface_alt_tree.xml");
		//faceClassifiers.push_back(CascadeClassifier());
		//faceClassifiers.back().load("data/haarcascade_frontalface_alt2.xml");
		//faceClassifiers.push_back(CascadeClassifier());
		//faceClassifiers.back().load("data/haarcascade_frontalface_default.xml");
		faceClassifiers.push_back(CascadeClassifier());
		faceClassifiers.back().load("data/haarcascade_profileface.xml");
	}
	void FindFaces(const Mat& mat, Faces& faces)
	{
		Mat src = mat;

		vector<Rect> objects;
		for (vector<CascadeClassifier>::iterator faceClassifier = faceClassifiers.begin(); faceClassifier != faceClassifiers.end(); ++faceClassifier)
		{
			faceClassifier->detectMultiScale(src, objects,1.1,4);
			for (vector<Rect>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				if (it->width > 1) // Minimum square size, else it's false positive or too far
				{
					Faces::iterator intersectionFound = faces.end();
					for (Faces::iterator face = faces.begin(); face != faces.end(); ++face)
					{
						Rect intersectionArea = (*it) & face->boundingBox;
						if (intersectionArea.width != 0)
							intersectionFound = face;
					}
					if (intersectionFound == faces.end())
						faces.push_back(*it);
					else
						intersectionFound->reliabilityFactor += 1;
				}
			}
			objects.clear();
		}

		//Preprocessing::ResizeByWidth(src, 200);
		//GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
		//Mat sobel;
		//Sobel(src, sobel, CV_64F, 1, 0, 5);

		//Preprocessing::ResizeByWidth(src, 1000, INTER_NEAREST);
		//Preprocessing::ResizeByWidth(sobel, 1000, INTER_NEAREST);
		//imshow("Face finding algorithm1", src);
		//imshow("Face finding algorithm2", sobel);
	}
};

#endif