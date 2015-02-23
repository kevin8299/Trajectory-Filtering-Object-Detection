#pragma once

#include "stdafx.h"

using namespace cv;
using namespace std;

ref class SpeedAdaptiveKalmanFilter
{
public:
	SpeedAdaptiveKalmanFilter(void);
	~SpeedAdaptiveKalmanFilter(void);

	float measureXDeviation;
	float measureYDeviation;

	float speedXDev;
	float speedYDev;

	float lastDeviation;
	int lastAdj;
	int incTime, decTime;
	float newDev;
	int first;

	int defaultMeasureDev;
	int noiseStdDev;
	int maxMove;
	int maxSpeed;

	vector<Point>* kalmanCorrect;

	KalmanFilter* KF;

	void kalmanInit();
	void kalmanDynamicAdjust( float error );
	void kalmanDynamicAdjust1(float error);
	Point filter( Mat_<float> measurement );
};

