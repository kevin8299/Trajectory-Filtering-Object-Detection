/**
  Real implementation of object detection with color and feature extraction
*/

#include "stdafx.h"
#include "FingerDetector.h"

/**
  Construction of this class
  @param: None
*/
FingerDetector::FingerDetector(void)
{
	fingerWidth = 10;
	fingerHeight = 40;
	//finger_cascade_name = new string("lbpcascade_frontalface.xml");
	finger_cascade_name = new string("lbp_finger_cascade.xml");
	finger_classifier = new CascadeClassifier();
	fingers = new vector<Rect>();
	hand = new vector<int>();
}

/**
  Instruction to guide user for next operations
  @param: None
*/
FingerDetector::~FingerDetector(void)
{
	free(finger_cascade_name);
	finger_cascade_name = NULL;

	free(fingers);
	fingers = NULL;

	free(hand);
	hand = NULL;

	finger_classifier -> ~CascadeClassifier();
	//free(finger_classifier);
	finger_classifier = NULL;
}

/**
  Init function to load some parameters
  @param w: image width
  @param h: image height
*/
void FingerDetector::detectorInit(int w, int h){
	frameWidth = w;
	frameHeight = h;
	if(! finger_classifier->load(*finger_cascade_name))
		cout << "Cannot load cascade file !" <<endl;
}	

/**
  Find hands with color range discrimination to get binary image
  @param image: input image
*/
void FingerDetector::findHand(Mat image)
{
	hand->clear();

	CvMemStorage* mStorage = cvCreateMemStorage(0);
	CvMemStorage* mDStorage = cvCreateMemStorage(0);
	int x1=0, x2=0, y1=0, y2=0, x=0, y=0, ww=0, hh=0;
	CvSize size = image.size();
	vector<int> v;
	CvSeq* contours = NULL;
	CvSeq* maxContour = NULL;
	
	Mat hsvImage = cvCreateImage(size, 8, 3);
	Mat hsvMask = cvCreateImage(size, 8, 1);
	CvScalar hsvMin = cvScalar(0, 30, 80, 0);
	CvScalar hsvMax = cvScalar(35, 165, 255, 0);
	cvtColor(image, hsvImage, CV_BGR2HSV);
	cvInRangeS(&IplImage(hsvImage), hsvMin, hsvMax, &IplImage(hsvMask));
	imshow("Color Determination", hsvMask);
	cvFindContours(&IplImage(hsvMask), mStorage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	
	int area = 0, maxArea = 0;
	while(contours)
	{
		area = fabs(cvContourArea(contours, CV_WHOLE_SEQ));
		if(area > maxArea)
		{
			maxArea = area;
			maxContour = contours;
		}
		contours = contours -> h_next;
	}

	int minstartx = 10000;
	int minstarty = 10000;
	int minendx = 10000;
	int minendy = 10000;
	
	if(maxArea > 500)
	{
		CvSeq* hull = cvConvexHull2(maxContour, 0, CV_CLOCKWISE, 0);
		CvSeq* defect = cvConvexityDefects(maxContour, hull, mDStorage);
		cvDrawContours(&IplImage(image), maxContour, CV_RGB(0, 200, 0), CV_RGB(0, 100, 0), 0, 1, 8, cvPoint(0, 0));
	
		CvPoint* pp = *(CvPoint**)cvGetSeqElem(hull, 0);
		x1 = pp -> x;
		y1 = pp -> y;
		x2 = pp -> x;
		y2 = pp -> y;
		
		for(int i=1; i < hull -> total; i++)
		{
			pp = *(CvPoint**)cvGetSeqElem(hull, i);
			x = pp -> x;
			y = pp -> y;
			
			if(x1 > x) x1 = x;
			if(x2 < x) x2 = x;
			if(y1 > y) y1 = y;
			if(y2 < y) y2 = y;
		}
		
		ww = abs(x2 - x1);
		hh = abs(y2 - y1);
		
		hsvImage.release();
		hsvMask.release();
	
		hand->push_back(x1);
		hand->push_back(y1);
		hand->push_back(ww);
		hand->push_back(hh);
		
		imshow("Binary Image to Detect Hands", image);
	}
}

/**
  Cut interest subimage off original 
  @param image: original video image
*/
void FingerDetector::findFingers(Mat image)
{
	if(hand->size() != 0){
		if(hand->at(0)>0 && hand->at(1)>0 && hand->at(2)>0 && hand->at(3)>0 && hand->at(0)+hand->at(2)< frameWidth && hand->at(1)+hand->at(3)<frameHeight && hand->at(2)>fingerWidth && hand->at(3)>fingerHeight)
		{
			Rect rect = cvRect(hand->at(0), hand->at(1), hand->at(2), hand->at(3));
			Mat mImage = image(rect);
			locateFinger(mImage);
		}
	}
}

/**
  Real implementation for detecting finger using output subimage rectangle containing hands
  @param m: input rectangle containing hands
*/
void FingerDetector::locateFinger(Mat m)
{
	Mat frame_gray;
	cvtColor(m, frame_gray, COLOR_BGR2GRAY);
	fingers->clear();
	
	finger_classifier->detectMultiScale(frame_gray, *fingers, 1.1, 40, 0, cv::Size(fingerWidth, fingerHeight));
	for(size_t i = 0; i < fingers->size(); i++)
	{
		rectangle(frame_gray, cvPoint(fingers->at(i).x, fingers->at(i).y), cvPoint(fingers->at(i).x + fingers->at(i).width, fingers->at(i).y + fingers->at(i).height ), Scalar(255,0,0));
		imshow("data",frame_gray);
	}
}



