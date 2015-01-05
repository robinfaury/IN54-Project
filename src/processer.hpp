#ifndef PROCESSER_HPP
#define PROCESSER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

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
		/* - - - - - Training - - - - - - - - - - - - - - - - - - - - */
		cout << "Training..." << endl;
		faceFinder.Train();
		for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
			(*it)->Train();
		cout << "Training done" << endl << endl;

		/* - - - - - Processing - - - - - - - - - - - - - - - - - - - - */
		int nbImg = 0;
		Mat currentImage, preprocessedImage, imageToShow;
		while (DigFolder("images", currentImage))
		{
			Faces faces;
			int angle = 30;
			float alpha = 0;
			Point2f center(currentImage.cols/2, currentImage.rows/2);

			for (int i=0; i<2*angle; i += 1)
			{
				if (i<angle)
					alpha = i;
				else
					alpha = angle - i;

				// Preprocess
				cout << "Preprocessing" << endl;
				currentImage.copyTo(preprocessedImage);

				Mat rot = getRotationMatrix2D(center, alpha, 1);
				warpAffine(preprocessedImage, preprocessedImage, rot, preprocessedImage.size());

				Preprocessing::Preprocess(preprocessedImage, 5);

				Faces newFaces;
				
				// Find faces
				cout << "Finding faces" << endl;
				faceFinder.FindFaces(preprocessedImage, newFaces);

				// Find eyes
				cout << "Finding eyes" << endl;
				for (Faces::iterator it = faces.begin(); it != faces.end(); ++it)
					eyeFinder.FindEyes(preprocessedImage, *it);
				cvtColor(preprocessedImage, imageToShow, COLOR_GRAY2RGB);

				//identify redundancy
				for (Faces::iterator it = newFaces.begin(); it != newFaces.end(); ++it)
				{
					bool match = false;
					Point2f newCoord = Preprocessing::RotateDegree(Point2f(it->boundingBox.x, it->boundingBox.y), center, -alpha);
					for (Faces::iterator it2 = faces.begin(); it2 != faces.end(); ++it2)
					{
						if (abs(it2->boundingBox.x - newCoord.x) < 50 && abs(it2->boundingBox.y - newCoord.y) < 50)
						{
							//it2->reliabilityFactor += it->reliabilityFactor;
							match = true;
							break;
						}
					}
					if (!match)
					{
						it->boundingBox.x = newCoord.x;
						it->boundingBox.y = newCoord.y;
						faces.push_back(*it);
					}
					
				}

				// Detect faces orientation
				cout << "Detecting orientations" << endl;
				for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
				{
					for (Faces::iterator itFace = newFaces.begin(); itFace != newFaces.end(); ++itFace)
						(*it)->DetectOrientation(imageToShow, *itFace);
				}

				// Decision taking
				cout << "Taking decisions:" << endl;
				for (Faces::iterator it = newFaces.begin(); it != newFaces.end(); ++it)
				{
					Verdict verdict = DecisionTaker::Decide(*it);

					// Display
					cout << " - Face located at " << it->boundingBox.x << ", " << it->boundingBox.y << " is ";
					if (verdict == Verdict::LOOKING_AT_THE_CAMERA)
					{
						rectangle(imageToShow, it->boundingBox, Scalar(0, 255, 0, 1), it->reliabilityFactor, 8, 0);
						cout << "looking at the camera" << endl;
					}
					else if (verdict == Verdict::NOT_LOOKING_AT_THE_CAMERA)
					{
						rectangle(imageToShow, it->boundingBox, Scalar(0, 0, 255, 1), it->reliabilityFactor, 8, 0);
						cout << "not looking at the camera" << endl;
					}
				}

				// Display
				cout << "Processing done" << endl << endl;
				namedWindow("Display Image", WINDOW_AUTOSIZE);
				imshow("Display Image", imageToShow);
				waitKey(1);
				preprocessedImage.release();
				imageToShow.release();
			}
			waitKey(1000);
			currentImage.release();
			preprocessedImage.release();
			imageToShow.release();
			destroyAllWindows();
			++nbImg;
		}

		if (nbImg == 0)
			cout << "No image data" << endl;
	}
};

#endif