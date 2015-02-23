#pragma once

using namespace cv;
using namespace std;

ref class LMSFilter
{
public:
	float mu;
	int begin;	
	float xErr, yErr, nx, ny;
	int step;

	Mat* ox;
	Mat* oy;
	Mat* wx;
	Mat* wy;
	Mat* x;
	Mat* y;
	//Mat* noisex;
	//Mat* noisey;
	vector<Point>* cachedNoisedVec;

	LMSFilter(void);
	~LMSFilter(void);

	void init();
	void filter(Point ptNoised, vector<Point>& filteredVec);
};

