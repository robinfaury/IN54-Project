#ifndef PROCESSER_HPP
#define PROCESSER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "facefinder.hpp"
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
		Mat image, preprocessedImage;
		while (DigFolder("images", image))
		{
			// Preprocess
			cout << "Preprocessing" << endl;
			preprocessedImage = image;
			Preprocessing::Preprocess(preprocessedImage);
			cvtColor(preprocessedImage, image, COLOR_GRAY2RGB);

			// Find faces
			cout << "Finding faces" << endl;
			Faces faces;
			faceFinder.FindFaces(preprocessedImage, faces);

			// Detect faces orientation
			cout << "Detecting orientations" << endl;
			for (list<OrientationDetector*>::iterator it = orientationDetectors.begin(); it != orientationDetectors.end(); ++it)
			{
				for (Faces::iterator itFace = faces.begin(); itFace != faces.end(); ++itFace)
					(*it)->DetectOrientation(image, *itFace);
			}

			// Decision taking
			cout << "Taking decisions:" << endl;
			for (Faces::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				Verdict verdict = DecisionTaker::Decide(*it);

				// Display
				cout << " - Face located at " << it->boundingBox.x << ", " << it->boundingBox.y << " is ";
				if (verdict == Verdict::LOOKING_AT_THE_CAMERA)
				{
					rectangle(image, it->boundingBox, Scalar(0, 255, 0, 1), it->reliabilityFactor, 8, 0);
					cout << "looking at the camera" << endl;
				}
				else if (verdict == Verdict::NOT_LOOKING_AT_THE_CAMERA)
				{
					rectangle(image, it->boundingBox, Scalar(0, 0, 255, 1), it->reliabilityFactor, 8, 0);
					cout << "not looking at the camera" << endl;
				}
			}

			// Display
			cout << "Processing done" << endl << endl;
			namedWindow("Display Image", WINDOW_AUTOSIZE);
			imshow("Display Image", image);
			waitKey(0);
			++nbImg;
			destroyAllWindows();
			image.release();
		}

		if (nbImg == 0)
			cout << "No image data" << endl;
	}
};

#endif