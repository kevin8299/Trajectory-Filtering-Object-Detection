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

using namespace std;
using namespace cv;

ref class FingerDetector
{
public:
	FingerDetector(void);
	~FingerDetector(void);

	int frameWidth, frameHeight;
	int fingerWidth, fingerHeight;

	String* finger_cascade_name;
	CascadeClassifier* finger_classifier;
	vector<Rect>* fingers;
	vector<int>* hand;


	void detectorInit(int, int);
	void findHand(Mat image);
	void findFingers(Mat image);
	void locateFinger(Mat m);

};

