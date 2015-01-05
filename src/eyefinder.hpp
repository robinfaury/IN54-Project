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
		
		/*equalizeHist(src, src);

		Mat left = src(Range(0, src.size[0] - 1), Range(0, src.size[1] - 1));


		int matchMethod = TM_SQDIFF_NORMED;
		Mat result;
		Mat templ = imread("data/eye_template2.png", IMREAD_GRAYSCALE);
		matchTemplate(left, templ, result, matchMethod);

		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		/// Localizing the best match with minMaxLoc
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED)
			matchLoc = minLoc;
		else
			matchLoc = maxLoc;

		//rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
		rectangle(left, Point(matchLoc.x - templ.cols / 2, matchLoc.y - templ.rows / 2), Point(matchLoc.x + templ.cols / 2, matchLoc.y + templ.rows / 2), Scalar::all(0), 2, 8, 0);


		imshow("Temp", src);
		imshow("Left", left);
		imshow("result", result);*/


		//Preprocessing::ResizeByWidth(src, 30, INTER_NEAREST);
		//Preprocessing::ResizeByWidth(src, 300, INTER_NEAREST);
		//Mat src = mat;

		/*Preprocessing::ResizeByWidth(src, 200);
		GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
		Mat sobel;
		Sobel(src, sobel, CV_64F, 0, 1, 5);
		resize(sobel, sobel, Size(0, 0), 0.6, 1, INTER_NEAREST);*/

		Mat test = src;
		//cvtColor(test, test, COLOR_BGR2GRAY);
		equalizeHist(test, test);
		Preprocessing::ResizeByWidth(test, 80, INTER_LINEAR);
		//test = test(Range(0, test.size[0] - 1), Range(0, test.size[1] / 2));

		vector<Rect> objects;
		for (vector<CascadeClassifier>::iterator eyeClassifier = eyeClassifiers.begin(); eyeClassifier != eyeClassifiers.end(); ++eyeClassifier)
		{
			eyeClassifier->detectMultiScale(src, objects, 1, 3);
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

		//sobel.convertTo(sobel, CV_8U);
		//vector<Vec3f> circles;

		/// Apply the Hough Transform to find the circles
		//HoughCircles(sobel, circles, HOUGH_GRADIENT, 1, 1, 100, 40, 0, 0);

		/// Draw the circles detected
		/*for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle(sobel, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(sobel, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}*/

		int matchMethod = TM_SQDIFF_NORMED;
		Mat result;
		Mat templ = imread("data/eye_template2.png", IMREAD_GRAYSCALE);
		matchTemplate(test, templ, result, matchMethod);

		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		/// Localizing the best match with minMaxLoc
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED)
			matchLoc = minLoc;
		else
			matchLoc = maxLoc;

		//rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
		rectangle(test, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);


		for (Eyes::iterator eye = face.eyes.begin(); eye != face.eyes.end(); ++eye)
			rectangle(src, eye->boundingBox, Scalar(255, 0, 0, 1), eye->reliabilityFactor, 8, 0);
		imshow("Temp", src);
		imshow("Sobel", test);
		//imshow("output", result);
		waitKey(0);
	}
};

#endif