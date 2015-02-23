#pragma once

#include "opencv2/video/background_segm.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <ctime>
#include <iostream>
#include <stdio.h>
#include "cv.h"

#include "FingerDetector.h"

using namespace std;
using namespace cv;

ref class ObjDetector
{
public:
	ObjDetector(void);

	void bgGMM(VideoCapture video);
	void colorAnalysis_decisionTree(VideoCapture video);
};

