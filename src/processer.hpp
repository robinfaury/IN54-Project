#ifndef PROCESSER_HPP
#define PROCESSER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include "facefinder.hpp"
#include "eyefinder.hpp"
#include "imgparser.hpp"
#include "orientationdetector.hpp"
#include "orientationdetector1.hpp"
#include "orientationdetector2.hpp"
#include "decisiontaker.hpp"

using namespace cv;
using namespace std;

// Main file used to do all the steps

class Processer : public ImgParser
{
private:
	FaceFinder faceFinder;
	EyeFinder eyeFinder;
	list<OrientationDetector*> orientationDetectors;
public:
	Processer()
	{
		orientationDetectors.push_back(new OrientationDetector1());
		orientationDetectors.push_back(new OrientationDetector2());
	}
	~Processer()
	{
		for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
			delete *it;
	}
	void Run()
	{
		fstream outputEyes;

		/* - - - - - Training - - - - - - - - - - - - - - - - - - - - */
		cout << "Training..." << endl;
		faceFinder.Train();
		eyeFinder.Train();
		for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
			(*it)->Train();
		cout << "Training done" << endl << endl;

		/* - - - - - Processing - - - - - - - - - - - - - - - - - - - - */
		int nbImg = 0;
		Mat currentImage, preprocessedImage;
		while (DigFolder("images", currentImage))
		{
			Faces faces;
			int angle = 70;
			float alpha = 0;
			Point2f center(currentImage.cols/2, currentImage.rows/2);

			cout << "Start processing" << endl;
			for (int alpha = -angle; alpha <= angle; alpha += 1)
			{

				if (alpha % 4 == 0)
					cout << "Search for angle " << alpha << endl;

				// Preprocess
				currentImage.copyTo(preprocessedImage);

				Mat rot = getRotationMatrix2D(center, alpha, 1);
				warpAffine(preprocessedImage, preprocessedImage, rot, preprocessedImage.size());

				Preprocessing::Preprocess(preprocessedImage, 5);

				Faces newFaces;
				
				// Find faces
				faceFinder.FindFaces(preprocessedImage, newFaces, alpha);

				// Find eyes
				for (Faces::iterator it = faces.begin(); it != faces.end(); ++it)
					eyeFinder.FindEyes(preprocessedImage, *it);

				//identify redundancy
				for (Faces::iterator it = newFaces.begin(); it != newFaces.end(); ++it)
				{
					bool match = false;
					Point2f centerNewFace = Preprocessing::RotateDegree(Point2f(it->boundingBox.x + it->boundingBox.width/2, it->boundingBox.y + it->boundingBox.height/2), center, +alpha);
					for (Faces::iterator it2 = faces.begin(); it2 != faces.end(); ++it2)
					{
						Point2f centerFace(it2->boundingBox.x + it2->boundingBox.width/2, it2->boundingBox.y + it2->boundingBox.height/2);
						if (abs(centerFace.x - centerNewFace.x) < 30 && abs(centerFace.y - centerNewFace.y) < 30)
						{
							it2->reliabilityFactor += it->reliabilityFactor;
							it2->alphaMax = it->alphaMin;
							match = true;
							break;
						}
					}
					if (!match)
					{
						it->boundingBox.x = centerNewFace.x - it->boundingBox.width/2;
						it->boundingBox.y = centerNewFace.y - it->boundingBox.height/2;
						faces.push_back(*it);
					}
					
				}
			}

			// Detect faces orientation
			cout << "Detecting orientations" << endl;
			for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
			{
				for (Faces::iterator itFace = faces.begin(); itFace != faces.end(); ++itFace)
					(*it)->DetectOrientation(currentImage, *itFace);
			}

			// Decision taking
			cout << "Taking decisions:" << endl;
			for (Faces::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				VerdictDetection verdictDetection = DecisionTaker::DecideDetection(*it);
				VerdictLooking verdictLooking = DecisionTaker::DecideLooking(*it);

				// Display
				cout << " - Face located at " << it->boundingBox.x << ", " << it->boundingBox.y << " is ";
				if (verdictDetection == VerdictDetection::IS_A_FACE)
				{
					rectangle(currentImage, it->boundingBox, Scalar(0, 255, 0, 1), it->reliabilityFactor/10, 8, 0);
					cout << "looking at the camera" << endl;
				}
				else if (verdictDetection == VerdictDetection::IS_NOT_A_FACE)
				{
					rectangle(currentImage, it->boundingBox, Scalar(0, 0, 255, 1), it->reliabilityFactor/10, 8, 0);
					cout << "not looking at the camera" << endl;
				}

				/*cout << " - Face located at " << it->boundingBox.x << ", " << it->boundingBox.y << " is ";
				if (verdictLooking == VerdictLooking::LOOKING_AT_THE_CAMERA)
				{
					rectangle(currentImage, it->boundingBox, Scalar(0, 255, 0, 1), it->reliabilityFactor/10, 8, 0);
					cout << "looking at the camera" << endl;
				}
				else if (verdictLooking == VerdictLooking::NOT_LOOKING_AT_THE_CAMERA)
				{
					rectangle(currentImage, it->boundingBox, Scalar(0, 0, 255, 1), it->reliabilityFactor/10, 8, 0);
					cout << "not looking at the camera" << endl;
				}*/
				Eye* best1 = 0;
				Eye* best2 = 0;
				Eye defaultEye(Rect(it->boundingBox.x, it->boundingBox.y, 0, 0));
				defaultEye.reliabilityFactor = 0;
				for (Eyes::iterator eye = it->eyes.begin(); eye != it->eyes.end(); ++eye)
				{
					rectangle(currentImage, eye->boundingBox, Scalar(255, 0, 0, 1), eye->reliabilityFactor / 8 + 1, 8, 0);
					if (best1 == 0)
					{
						best1 = &(*eye);
					}
					else if (best2 == 0)
					{
						best2 = &(*eye);
					}
					else if (eye->reliabilityFactor > best2->reliabilityFactor)
					{
						best2 = &(*eye);
					}
					if (best1 != 0 && best2 != 0 && best1->reliabilityFactor < best2->reliabilityFactor)
					{
						Eye* bestT = best1;
						best1 = best2;
						best2 = bestT;
					}
					//outputEyes << 
				}
				if (best2 == 0)
				{
					best2 = &defaultEye;
					if (best1 == 0)
						best1 = &defaultEye;
				}
				outputEyes.open("data/outputeyes.txt", std::ios_base::app | std::ios_base::out);
				if (verdictDetection == VerdictDetection::IS_A_FACE)
					outputEyes << "1";
				else
					outputEyes << "0";
				outputEyes << ", ";
				outputEyes << it->alphaMin;
				outputEyes << ", ";
				outputEyes << it->alphaMax;
				outputEyes << ", ";

				outputEyes << it->boundingBox.x;
				outputEyes << ", ";
				outputEyes << it->boundingBox.y;
				outputEyes << ", ";
				outputEyes << it->boundingBox.width;
				outputEyes << ", ";
				outputEyes << it->boundingBox.height;
				outputEyes << ", ";
				outputEyes << it->reliabilityFactor;
				outputEyes << ", ";

				outputEyes << best1->boundingBox.x - it->boundingBox.x;
				outputEyes << ", ";
				outputEyes << best1->boundingBox.y - it->boundingBox.y;
				outputEyes << ", ";
				outputEyes << best1->boundingBox.width;
				outputEyes << ", ";
				outputEyes << best1->boundingBox.height;
				outputEyes << ", ";
				outputEyes << best1->reliabilityFactor;
				outputEyes << ", ";

				outputEyes << best2->boundingBox.x - it->boundingBox.x;
				outputEyes << ", ";
				outputEyes << best2->boundingBox.y - it->boundingBox.y;
				outputEyes << ", ";
				outputEyes << best2->boundingBox.width;
				outputEyes << ", ";
				outputEyes << best2->boundingBox.height;
				outputEyes << ", ";
				outputEyes << best2->reliabilityFactor;
				outputEyes << ", " << endl;
				outputEyes.close();

			}

			// Display
			cout << "Processing done" << endl << endl;
			namedWindow("Display Image", WINDOW_AUTOSIZE);
			imshow("Display Image", currentImage);
			waitKey(0);
			currentImage.release();
			preprocessedImage.release();
			destroyAllWindows();
			
			++nbImg;
		}
		outputEyes.close();
		if (nbImg == 0)
			cout << "No image data" << endl;
	}
};

#endif