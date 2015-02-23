#pragma once

using namespace cv;
using namespace std;

ref class NNFilter
{
public:
	NNFilter(void);
	~NNFilter(void);

	CvANN_MLP* bp;
	CvANN_MLP_TrainParams* param;

	void init(vector<Point> mouseNoisedVec, vector<Point> mouseClearVec);
	void filter(Mat* testIn, Mat* testOut );
};
