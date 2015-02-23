/**
  Object detector contains: 
     (1) background subtraction algorithm of GMM
	 (2) mixture algorithm of color analysis and decision tree
*/
#include "stdafx.h"
#include "ObjDetector.h"

/**
  Construction of this class
  @param: None
*/
ObjDetector::ObjDetector(void)
{
}

/**
  Detect the finger object with background subtraction of GMM
  @param video: is a handler for video file
*/
void ObjDetector::bgGMM(VideoCapture video){
	Mat frame, mask;
	BackgroundSubtractorMOG bgSubtractor(3, 3, 0.1, false);

	long totolFrameNumber = (long)video.get(CV_CAP_PROP_FRAME_COUNT);
	//cout << "totolFrameNumber : " << totolFrameNumber << endl;

	for(int i = 0; i < totolFrameNumber - 1; i++)
	{
		video >> frame;
		cvtColor(frame, frame, CV_RGB2GRAY);
		imshow("GMM Background Subtraction", frame);
		bgSubtractor(frame, mask, 0.1);
		imshow("mask", mask);
		if('c' == waitKey(10))
			break;

		cout << "Frame Number: " << i << endl;
	}

	cout << "bgGMM has read all frames" << endl;
}

/**
  Detect the finger object with mixture algorithm of color analysis and decision tree 
  @param video: is a handler for video file
*/
void ObjDetector::colorAnalysis_decisionTree(VideoCapture video)
{
	FingerDetector d;
	Mat frame, mask;

	double totolFrameNumber = video.get(CV_CAP_PROP_FRAME_COUNT);
	double frameWidth = video.get(CV_CAP_PROP_FRAME_WIDTH);
    double frameheight = video.get(CV_CAP_PROP_FRAME_HEIGHT);
	//cout << "totolFrameNumber : " << totolFrameNumber << " frameWidth: " << frameWidth << " frameheight: " << frameheight << endl;

	d.detectorInit(frameWidth, frameheight);

	for(int i = 0; i < totolFrameNumber - 1; i++)
	{
		video >> frame;	
		//CvSize mOriImageSize = frame.size();
		//imshow("CapturedImage", frame);
		if('c' == waitKey(10))
			break;

		d.findHand(frame);
		for(int i=0; i < d.hand->size() / 4; i++)
			d.findFingers(frame);

		cout << "Frame Number: " << i << endl;
	}

	cout << "colorAnalysis_decisionTree has read all frames" << endl;
}
