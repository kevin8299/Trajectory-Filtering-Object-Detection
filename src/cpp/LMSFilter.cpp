/**
  Realtime filtering with LMS algorithm
*/

#include "stdafx.h"
#include "LMSFilter.h"


/**
  Construction of this class
  @param: None
*/
LMSFilter::LMSFilter(void)
{	
	step = 3;
	wx = new Mat(step, 1, CV_32FC1);
	wy = new Mat(step, 1, CV_32FC1);
	ox = new Mat();
	oy = new Mat();

	x = new Mat(1, step,CV_32FC1);
	y = new Mat(1, step,CV_32FC1);
	//noisex = new Mat(1, step,CV_32FC1);
	//noisey = new Mat(1, step,CV_32FC1);

	cachedNoisedVec = new vector<Point>();
}

/**
  Deconstruction of this class
  @param: None
*/
LMSFilter::~LMSFilter(void)
{	
	free(wx);
	wx = NULL;

	free(wy);
	wy = NULL;

	free(ox);
	ox = NULL;

	free(oy);
	oy = NULL;

	free(x);
	x = NULL;

	free(y);
	y = NULL;
}

/**
  Init to set up parameters
  @param: None
*/
void LMSFilter::init()
{
	mu = 0.0000001;
	begin = 0;

	for(int i=0; i<step; i++)
	{
		wx->at<float>(i, 0) = 0;
		wy->at<float>(i, 0) = 0;
	}
}
		
/**
  Filter implementation based on LMS
  @param ptNoised: noised data in
  @param filteredVec: filtered dataset stored for showing in image console
*/
void LMSFilter::filter(Point ptNoised, vector<Point>& filteredVec){ //, vector<Point>& filteredYVec

	if(cachedNoisedVec->size() < step)
		cachedNoisedVec->push_back(ptNoised);	
	else
	{
		for(int j=begin; j<begin + step; j++)
		{
			x->at<float>(0, j - begin) = cachedNoisedVec->at(j).x;
			y->at<float>(0, j - begin) = cachedNoisedVec->at(j).y;
			//noisex.at<float>(0, j - beginning) = noiseX;
			//noisey.at<float>(0, j - beginning) = noiseY;
		}
				

		*ox = (*x) * (*wx);  // ox = noisex * wx;
		*oy = (*y) * (*wy);  // oy = noisey * wy;
				
		Point out(ox->at<float>(0 , 0), oy->at<float>(0 , 0));
		filteredVec.push_back(out);
				
		xErr = ptNoised.x - ox->at<float>(0 , 0);
		yErr = ptNoised.y - oy->at<float>(0 , 0);
				
		//Point out1(xErr, yErr);
		//filteredYVec.push_back(out1);
				
		*wx = *wx + 2 * mu * xErr * x->t();
		*wy = *wy + 2 * mu * yErr * y->t();
				
		cachedNoisedVec->push_back(ptNoised);
		begin ++;	
	}
}