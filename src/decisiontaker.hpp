#ifndef DECISIONTAKER_HPP
#define DECISIONTAKER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"

using namespace cv;
using namespace std;

// Contains the algorithm taking the decision to set a state on a face from its extracted data

enum Verdict { LOOKING_AT_THE_CAMERA, NOT_LOOKING_AT_THE_CAMERA, TOO_FAR };

class DecisionTaker
{
public:
	static Verdict Decide(Face& face)
	{
		Verdict verdict = Verdict::TOO_FAR;
		if (rand() % 2 == 0)
			verdict = Verdict::LOOKING_AT_THE_CAMERA;
		if (rand() % 2 == 0)
			verdict = Verdict::NOT_LOOKING_AT_THE_CAMERA;
		// ?
		return verdict;
	}
};

#endif