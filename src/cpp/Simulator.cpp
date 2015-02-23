/**
  Trajectory Filtering Simulator Console for various online/offline filters
*/

#include "stdafx.h"
#include "Simulator.h"

struct mouse_info_struct { int x,y; };
struct mouse_info_struct mouse_info = {-1,-1};  //, last_mouse

/**
  Set up for handling mouse event 
  @param event: event id
  @param x: coordinate x
  @param y: coordinate y
  @param flags: flag indicator
  @param param: parameters
*/
void on_mouse(int event, int x, int y, int flags, void* param)
{
	//if(event == CV_EVENT_LBUTTONUP)
	{
		//last_mouse = mouse_info;
		mouse_info.x = x;
		mouse_info.y = y;
	}
	//}
}

/**
  Construction of this class
  @param: None
*/
Simulator::Simulator(void)
{
	dataLen = 1000;
	noiseStdDev = 20;
	winSize = 400;
	dataLen = 1000;
	sampleTime = 10;

	noises = cvCreateMat( 2, dataLen, CV_32F );
	noisesLarge = cvCreateMat( 2, dataLen, CV_32F );

	noiseInit();

	img = new Mat(winSize, winSize, CV_8UC3);
	*img = Scalar::all(0);

	mouseClearVec = new vector<Point>();
	mouseNoisedVec = new vector<Point>();
	filteredVec = new vector<Point>();

	windowName = new string("TrajectoryFilteringDemo");
	namedWindow(*windowName);
	setMouseCallback(*windowName, on_mouse, 0);
}

/**
  Deconstruction of this class
  @param: None
*/
Simulator::~Simulator(void)
{
	free(img);
	img = NULL;

	free(mouseClearVec);
	mouseClearVec = NULL;

	free(mouseNoisedVec);
	mouseNoisedVec = NULL;

	free(filteredVec);
	filteredVec = NULL;

	free(windowName);
	windowName = NULL;
}

/**
  Drawing special crosses for showing
  @param img: image for drawing
  @param center: drawing point on image
  @param color: color of the cross
  @param d: cross length
*/
void Simulator::drawCross(Mat img, Point center, Scalar color, int d){
	line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color);  
	line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color) ;
}

/**
  Generation of a set of noises for simulation
  @param: None
*/
void Simulator::noiseInit(){
	CvRandState rng;
	
	rng.state = cvRNG( cvGetTickCount() );
	cvRandInit( &rng, 0, 20, 1, CV_RAND_NORMAL );
	cvRandArr( &rng.state, noises, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar( noiseStdDev ) );

	Sleep( 100 );

	rng.state = cvRNG( cvGetTickCount() );
	cvRandInit( &rng, 0, 20, 1, CV_RAND_NORMAL );
	cvRandArr( &rng.state, noisesLarge, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar( noiseStdDev ) );

}

/**
  Store the simation dataset of clear and nosied data
  @param clearFile: file name of clear dataset
  @param noisedFile: file name of noised dataset
*/
void Simulator::createData(char* clearFile, char* noisedFile){
	
	float noiseX, noiseY;
	ofstream foutClear, foutNoised;

	foutClear.open( clearFile );
	foutNoised.open( noisedFile );

	mouseClearVec->clear();
	mouseNoisedVec->clear();

	cout<<"Creating offline dataset process is triggered. Please press SPACE key to start inputting. "<<endl;

	*img = Scalar::all( 0 );	
	imshow( *windowName, *img );
	if(  cvWaitKey( -1 ) == 32 )  // space as start
	{
		for( int i=0; i < dataLen; i++ )
		{
			Sleep( sampleTime );
				
			noiseX = cvmGet( noises, 0, i );
			noiseY = cvmGet( noises, 1, i );
			int xNoised = ( int )( mouse_info.x + noiseX );
			int yNoised = ( int )( mouse_info.y + noiseY );
				
			foutNoised << xNoised << endl;
			foutNoised << yNoised << endl;
				
			foutClear << mouse_info.x << endl;
			foutClear << mouse_info.y << endl;
				
			Point pt( xNoised, yNoised );
			mouseNoisedVec->push_back( pt );
				
			Point pt1( mouse_info.x, mouse_info.y );
			mouseClearVec->push_back( pt1 );				
				
			drawCross( *img, pt, Scalar( 0, 0, 255 ), 5 );
			
			for( int i = 0; i < mouseNoisedVec->size() - 1; i++ )
			{
				line( *img, (*mouseNoisedVec)[i], (*mouseNoisedVec)[i+1], Scalar( 255, 255, 255), 1 );
				line( *img, (*mouseClearVec)[i], (*mouseClearVec)[i+1], Scalar( 0, 0, 255), 1 );
			}
			imshow( *windowName, *img );
			
			if( cvWaitKey( 100 ) == 32 )  // space as stop
			{
				foutNoised.close();
				foutClear.close();
				break;
			}
		}
	}

}

/**
  Show filtered result clearly for comparision
  @param: None
*/
void Simulator::showResult()
{
	cout << "showResult is Not realized now " << endl;
	//img1 = Scalar::all( 0 );
 //   for( int i = 0; i < kalmanCorrect.size() - 1; i++ )
	//{
	//	line( img1, kalmanCorrect[i], kalmanCorrect[i+1], Scalar( 0, 255, 0 ), 1 );
	//	circle( img1, kalmanCorrect[i], 4, Scalar( 0, 255, 0 ), 2 );
	//}

	//for( int i = 0; i<mouseClearVec.size() - 1; i++ )
	//{
	//	line( img1, mouseClearVec[i], mouseClearVec[i+1], Scalar( 0, 0, 255 ), 1 );
	//	circle( img1, mouseClearVec[i], 4, Scalar( 0, 255, 0 ), 2 );	
	//}
	//
	//imshow( "Result", img1 );
	//waitKey( -1 );

}

/**
  Read dataset stored in files
  @param filePath: file to read
  @param vec: dataset to store the read out data
*/
void Simulator::readData(String filePath, vector<Point> &vec)
{
	ifstream fin(filePath);
	string str;
	int x,y;
	
	while(getline(fin, str))
	{
		int x = atoi(str.c_str());
		
		getline(fin, str);
		int y = atoi(str.c_str());
		
		Point pt(x, y);
		vec.push_back(pt);
	}
}

/**
  Show the read out dataset
  @param: None
*/
void Simulator::dataReadShow(){

		//read the noised dataset
		mouseNoisedVec->clear();
		readData("datasetNoised.txt", *mouseNoisedVec );
		for( int i = 0; i < mouseNoisedVec->size() - 1; i++ )
		{
			line( *img, (*mouseNoisedVec)[i], (*mouseNoisedVec)[i+1], Scalar( 255, 255, 255 ), 1);
		}
		
		//read the original clear dataset
		mouseClearVec->clear();
		readData("datasetClear.txt", *mouseClearVec );
		for( int i = 0; i < mouseClearVec->size() - 1; i++ )
		{
			line( *img, (*mouseClearVec)[i], (*mouseClearVec)[i+1], Scalar( 0, 0, 255 ), 1);
		}		
		
		imshow( *windowName , *img );
		cvWaitKey( 10 );	
}

/**
  Speed adaptive KF for realtime filtering 
  @param: None
*/
void Simulator::realtimeFiltering()
{
	float noiseX, noiseY;
	Point ptEstimated;
	char code = ( char ) -1;
	Mat prediction;

	Mat_<float> measurement( 2, 1 );
	measurement.setTo( Scalar( 0 ) );

	SpeedAdaptiveKalmanFilter kf;
	kf.kalmanInit();
	
	//data array clear
	mouseClearVec->clear();
	kf.kalmanCorrect->clear();
	mouseNoisedVec->clear();

	*img = Scalar::all(0);
	imshow(*windowName, *img);
	cvWaitKey(10);
		
	for( int i = 0; i < dataLen; i++ )
	{
		prediction = kf.KF->predict();
		Point predictPt( prediction.at<float>(0), prediction.at<float>(1) );
		
		noiseX = cvmGet(noises, 0, i);
		noiseY = cvmGet(noises, 1, i);
			
		Sleep( sampleTime );
		Point ptReal( mouse_info.x, mouse_info.y );
		Point ptNoised( ptReal.x + noiseX, ptReal.y + noiseY );
			
		measurement( 0 ) = ptNoised.x;
		measurement( 1 ) = ptNoised.y; 
			
		ptEstimated = kf.filter( measurement );
			
		Point measPt( measurement( 0 ), measurement( 1 ) );
		mouseNoisedVec->push_back( measPt );
		mouseClearVec->push_back( ptReal );
		
		//showing
		circle( *img, measPt, 4, Scalar( 150, 180, 255), 4 );
		for( int i =0; i < mouseNoisedVec->size() - 1; i++)
		{
			line( *img, (*mouseNoisedVec)[i], (*mouseNoisedVec)[i+1], Scalar( 255, 255, 255 ), 1 );
			line( *img, (*mouseClearVec)[i], (*mouseClearVec)[i+1], Scalar( 0, 0, 255 ), 1 );
			circle( *img, (*mouseClearVec)[i], 4, Scalar( 200, 100, 50 ), 2 );
		}
			
		circle( *img, ptEstimated, 4, Scalar( 0, 255, 0), 2);
		for( int i = 0; i < kf.kalmanCorrect->size() - 1; i++)
		{
			line( *img, (*(kf.kalmanCorrect))[i],(*(kf.kalmanCorrect))[i+1],Scalar( 0, 255, 0 ), 2);
		}
			
		imshow(*windowName, *img);
		
		//key handling
		code = ( char )waitKey( 100 ); 
		if( code == 'w' )
		{
			waitKey( -1 );
			continue;
		}
		
		if( code == 's' )
			showResult();
		
		if( code == 'c')
			break;
		}
}

/**
  Speed adaptive KF for realtime filtering of stored dataset, which is used for debugging the filter
  @param: None
*/
void Simulator::storedDataKFFiltering(){
	Point ptEstimated;
	char code = ( char ) -1;	
	Mat_<float> measurement( 2, 1 );

	SpeedAdaptiveKalmanFilter kf;
	kf.kalmanInit();

	mouseClearVec->clear();
	kf.kalmanCorrect->clear();

	*img = Scalar::all(0);
	imshow(*windowName, *img);
	cvWaitKey(10);

	dataReadShow();
	
	for( int i = 0; i < mouseNoisedVec->size() - 1; i++ )
	{ 
		Sleep(sampleTime);
			
		measurement(0) = (*mouseNoisedVec)[i].x;
		measurement(1) = (*mouseNoisedVec)[i].y;
			
		ptEstimated = kf.filter( measurement );
			
		circle( *img, ptEstimated, 4, Scalar( 0, 255, 0 ), 2 );
		for( int i =0; i < kf.kalmanCorrect->size() - 1; i++ )
		{
			line( *img, (*(kf.kalmanCorrect))[i], (*(kf.kalmanCorrect))[i+1], Scalar( 0, 255, 0), 2 );
		}
		imshow( *windowName, *img );
		code = waitKey( 100 );

		if( code == 'c' ) 
			break;
	}
}

/**
  Neutal Network filter for offline filtering
  @param: None
*/
void Simulator::nnFiltering(){

	*img = Scalar::all(0);
	imshow(*windowName, *img);
	cvWaitKey(10);

	dataReadShow();

	imshow( *windowName, *img );
	cout << "Press 's' to start training ...... " << endl;
	int c = -1 ;
	while(true){
		c = getchar();
		if( c == 's' ) 
			break;
	}

	cout << "NN is training ...... " << endl;
	NNFilter nnf;
	nnf.init(*mouseNoisedVec, *mouseClearVec);
	cout << "Training is OVER, predicting running ...... " << endl;
		
	Mat testIn( 1, 2, CV_32FC1 );
	Mat testOut( 1, 2, CV_32FC1 );
	Point pt;
	Point pt0(-1, -1);

	int len = mouseNoisedVec->size();	
	for( int i = 0; i < len ; i++ )
	{
		testIn.at<float>( 0, 0 ) = (*mouseNoisedVec)[i].x;//trainData.at<float>( i, 0 );
		testIn.at<float>( 0, 1 ) = (*mouseNoisedVec)[i].y;//trainData.at<float>( i, 1 );
		
		nnf.filter( &testIn, &testOut );
			
		pt.x = testOut.at<float>( 0, 0 );
		pt.y = testOut.at<float>( 0, 1 );
			
		circle( *img, pt, 2, Scalar( 0, 255, 255 ), 3 );
		if(pt0.x != -1)
			line(*img, pt0, pt, Scalar(0, 255, 0), 1);
			
		pt0 = pt;
		//cout << "test: " << testIn.at<float>( 0, 0 ) << " " << testIn.at<float>( 0, 1 ) << " ------ " << pt.x << " " <<pt.y <<endl;
		imshow( *windowName, *img );
		int code = waitKey( 100 );

		if( code == 'c' ) 
			break;

	}
	//cout << "NN filtering is OVER, press any key to EXIT. " << endl;
	//cvWaitKey( -1 );
}

/**
  LMS for realtime filtering
  @param: None
*/
void Simulator::lmsFiltering(){
	LMSFilter lmsf;
	float noiseX, noiseY;

	*img = Scalar::all(0);
	imshow(*windowName, *img);
	cvWaitKey(10);
		
	mouseNoisedVec->clear();
	mouseClearVec->clear();

	lmsf.init();

	for(int i=0; i<dataLen; i++)
	{
		//test data construction
		noiseX = cvmGet(noises, 0, i);
		noiseY = cvmGet(noises, 1, i);
		Point ptReal(mouse_info.x, mouse_info.y);
		Point ptNoised(ptReal.x + noiseX, ptReal.y + noiseY);

		//filtering
		lmsf.filter( ptNoised, *filteredVec);
			
		//push back
		mouseNoisedVec->push_back(ptNoised);
		mouseClearVec->push_back(ptReal);
			
		//for showing
		if(mouseNoisedVec->size() > 0)
		{
			for(int i = 0; i < mouseNoisedVec->size() - 1; i++)
			{
				line(*img, (*mouseNoisedVec)[i], (*mouseNoisedVec)[i + 1], Scalar(0, 255, 0), 1);
				line(*img, (*mouseClearVec)[i], (*mouseClearVec)[i + 1], Scalar(255, 0, 0), 1);
				circle(*img, (*mouseClearVec)[i], 4, Scalar(255, 0, 0));
				circle(*img, (*mouseNoisedVec)[i], 4, Scalar(0, 255, 0));
			}
		}
			
		if(filteredVec->size() > 0)
		{
			for(int i = 0; i < filteredVec->size() - 1; i++)
			{
				line(*img, (*filteredVec)[i], (*filteredVec)[i + 1], Scalar(0, 0, 255), 1);
				circle(*img, (*filteredVec)[i], 4, Scalar(0, 0, 255));
			}
		}	

		imshow(*windowName, *img);
		int code = waitKey( 100 );

		if( code == 'c' ) 
			break;	
	}
}




