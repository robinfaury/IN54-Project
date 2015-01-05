#ifndef DECISIONTAKER_HPP
#define DECISIONTAKER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "face.hpp"

using namespace cv;
using namespace std;

// Contains the algorithm taking the decision to set a state on a face from its extracted data

enum VerdictDetection { IS_A_FACE, IS_NOT_A_FACE };
enum VerdictLooking { LOOKING_AT_THE_CAMERA, NOT_LOOKING_AT_THE_CAMERA };

class DecisionTaker
{
public:
	static VerdictDetection DecideDetection(Face& face)
	{
		VerdictDetection verdict = VerdictDetection::IS_NOT_A_FACE;
		if (face.reliabilityFactor > 5)
			verdict = VerdictDetection::IS_A_FACE;

		return verdict;
	}

	static VerdictLooking DecideLooking(Face& face)
	{
		VerdictLooking verdict = VerdictLooking::NOT_LOOKING_AT_THE_CAMERA;
		if (rand() % 2 == 0)
			verdict = VerdictLooking::LOOKING_AT_THE_CAMERA;

		return verdict;
	}
};

#endif