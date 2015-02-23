#pragma once
#include "stdafx.h"
#include "NNFilter.h"
#include "LMSFilter.h"
#include "SpeedAdaptiveKalmanFilter.h"

using namespace cv;
using namespace std;

ref class Simulator
{
public:
	Simulator(void);
	~Simulator(void);

	CvMat* noises;
	CvMat* noisesLarge;
	vector<Point>* mouseClearVec;
	vector<Point>* mouseNoisedVec;
	vector<Point>* kalmanCorrect;
	vector<Point>* filteredVec;

	Mat* img;
	string* windowName;

	int dataLen;
	int noiseStdDev;
	int winSize;
	int sampleTime;

	void noiseInit();
	void createData(char* clearFile, char* noisedFile);
	void readData(String filePath, vector<Point> &vec);
	void dataReadShow();
	void showResult();
	void drawCross(Mat img, Point center, Scalar color, int d);

	//filtering methods
	void nnFiltering();
	void lmsFiltering();
	void realtimeFiltering();
	void storedDataKFFiltering();
};

