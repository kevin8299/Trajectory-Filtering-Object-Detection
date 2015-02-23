/**
  Neural network modeling for offline noise filtering
*/

#include "stdafx.h"
#include "NNFilter.h"

/**
  Construction of this class
  @param: None
*/
NNFilter::NNFilter()
{		
	bp = new CvANN_MLP;
	param = new CvANN_MLP_TrainParams;
}

/**
  Deconstruction of this class
  @param: None
*/
NNFilter::~NNFilter()
{		
	free(bp);
	bp = NULL;

	free(param);
	param = NULL;
}

/**
  Init to set parameters and train NN
  @param mouseNoisedVec: noised dataset 
  @param mouseClearVec: clear dataset
*/
void NNFilter::init(vector<Point> mouseNoisedVec, vector<Point> mouseClearVec){
	int len = mouseNoisedVec.size();
	Mat trainData( len, 2, CV_32FC1 );
	Mat trainLabel( len, 2, CV_32FC1 );
		
	for( int i = 0; i < len; i++ )
	{
		trainData.at<float>( i, 0 ) = mouseNoisedVec[i].x; 
		trainData.at<float>( i, 1 ) = mouseNoisedVec[i].y; 
			
		trainLabel.at<float>( i, 0 ) = mouseClearVec[i].x; 
		trainLabel.at<float>( i, 1 ) = mouseClearVec[i].y; 
	}
		
	bp = new CvANN_MLP;
	param = new CvANN_MLP_TrainParams;
	param->term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 1000, 1 );
	param->train_method = CvANN_MLP_TrainParams::BACKPROP;
	param->bp_dw_scale = 0.1;
	param->bp_moment_scale = 0.1;
		
	Mat_<int> layerSize( 1, 3);
	layerSize.at<int>( 0, 0 ) = 2;
	layerSize.at<int>( 0, 1 ) = 8;
	layerSize.at<int>( 0, 2 ) = 2;
		
	bp->create( layerSize, CvANN_MLP::SIGMOID_SYM );
	bp->train( trainData, trainLabel, Mat(), Mat(), *param );
}

/**
  Noise filtering with trained NN 
  @param testIn: noised data in
  @param testOut: filtered data out
*/
void NNFilter::filter(Mat* testIn, Mat* testOut){
	bp->predict( *testIn, *testOut );
}
