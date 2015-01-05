#ifndef EYECLASSIFIER_HPP
#define EYECLASSIFIER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "face.hpp"
#include "preprocessing.hpp"

using namespace cv;
using namespace std;

// Contains eye recognition algorithm

class EyeClassifier
{
private:
	int type;
	CascadeClassifier cascadeClassifier;
public:
	EyeClassifier(int t) : type(t)
	{

	}
	~EyeClassifier()
	{

	}
	void Train()
	{
		if (type == 1)
		{
			cascadeClassifier.load("data/haarcascade_eye.xml");
		}
	}
	void FindEyes(const Mat& matFace, vector<Rect>& objects)
	{
		if (type == 1)
		{
			cascadeClassifier.detectMultiScale(matFace, objects, 1.1, 4);
			return;
		}
		Mat src = matFace;

		equalizeHist(src, src);
		Preprocessing::ResizeByWidth(src, 80, INTER_LINEAR);

		int matchMethod = type - 2;
		Mat result;
		Mat templ = imread("data/eye_template2.png", IMREAD_GRAYSCALE);
		//Preprocessing::ResizeByWidth(templ, (templ.size[0] * src.size[0]) / 80, INTER_LINEAR);

		matchTemplate(src, templ, result, matchMethod);

		//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		/// Localizing the best match with minMaxLoc
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;

		}
		else
		{
			matchLoc = maxLoc;
			minVal = maxVal;
		}

		float ratio = matFace.size[0] / 80.0f;
		if (minVal < 0.07f)
			objects.push_back(Rect(matchLoc * ratio, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows) * ratio));
		
		rectangle(result, matchLoc * ratio, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows) * ratio, Scalar(127, 0, 0, 1), FILLED, 8, 0);


		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;

		}
		else
		{
			matchLoc = maxLoc;
			minVal = maxVal;
		}

		ratio = matFace.size[0] / 80.0f;
		if (minVal < 0.07f)
			objects.push_back(Rect(matchLoc * ratio, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows) * ratio));
		/*rectangle(matFace, matchLoc * ratio, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows) * ratio, Scalar(0, 0, 0, 1), 1, 8, 0);
		imshow("matFace", matFace);
		waitKey(0);*/
	}
};

#endif