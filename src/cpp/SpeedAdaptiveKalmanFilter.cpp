/**
  Kalman filter with speed adaptive implementation
*/

#include "stdafx.h"
#include "SpeedAdaptiveKalmanFilter.h"

/**
  Construction of this class
  @param: None
*/
SpeedAdaptiveKalmanFilter::SpeedAdaptiveKalmanFilter(void)
{
	measureXDeviation = 10;
	measureYDeviation = 10 ;

	speedXDev = .1;
	speedYDev = .1;

	measureXDeviation = 10;
	measureYDeviation = 10 ;

	speedXDev = .1;
	speedYDev = .1;

	lastDeviation = 0.7 * noiseStdDev;
	lastAdj = -1;
	incTime = 0, decTime = 0;
	newDev = 0;
	first = 1;

	defaultMeasureDev = 1;
	noiseStdDev = 20;
	maxMove = 50;
	maxSpeed = 50;

	kalmanCorrect = new vector<Point>();
	KF = new KalmanFilter(4, 2);
}

/**
  Deconstruction of this class
  @param: None
*/
SpeedAdaptiveKalmanFilter::~SpeedAdaptiveKalmanFilter(void)
{
	free(kalmanCorrect);
	kalmanCorrect = NULL;

	free(KF);
	KF = NULL;
}

/**
  Init to construct parameters of KF
  @param: None
*/
void SpeedAdaptiveKalmanFilter::kalmanInit()
{
	KF->transitionMatrix = *( Mat_<float>(4,4) << 1,0,1,0, 0,1,0,1, 0,0,1,0, 0,0,0,1 );
    KF->measurementMatrix = *( Mat_<float>(2,4) << 1,0,0,0, 0,1,0,0 );

    KF->measurementNoiseCov = *( Mat_<float>(2,2) << defaultMeasureDev,0, 0,defaultMeasureDev );
	KF->processNoiseCov = *( Mat_<float>(4,4) << .1,0,0,0, 0,.1,0,0, 0,0,.1,0, 0,0,0,.1 );
}

/**
  Function for adjusting KF parameters dynamically depending on errors of last correcting
  @param error: error of last KF adjusting
*/
void SpeedAdaptiveKalmanFilter::kalmanDynamicAdjust1(float error)
{
	float measureAdj = 1;
	float errorTh1 = 0.7 * noiseStdDev;
	float errorTh2 = 0.7 * 0.7 * noiseStdDev;

	if( error > errorTh1 )
	{
		measureXDeviation += measureAdj;
		measureYDeviation += measureAdj;
	
		if( measureXDeviation > 100 )
		{
			measureXDeviation = 100;
		}
	
		if( measureYDeviation > 100 )
		{
			measureYDeviation = 100;
		}

		KF->measurementNoiseCov = *( Mat_<float>(2,2) << measureXDeviation,0, 0,measureYDeviation );
	}

	if( error < errorTh2 )
	{
		measureXDeviation -= measureAdj;
		measureYDeviation -= measureAdj;
	
		if( measureXDeviation > 100 )
		{
			measureXDeviation = 100;
		}
	
		if( measureYDeviation > 100 )
		{
			measureYDeviation = 100;
		}

		KF->measurementNoiseCov = *( Mat_<float>(2,2) << measureXDeviation,0, 0,measureYDeviation );
	}
}

/**
  Function for adjusting KF parameters dynamically depending on errors of last correcting
  @param error: error of last KF adjusting
*/
void SpeedAdaptiveKalmanFilter::kalmanDynamicAdjust( float error )
{
	float measureAdj = 1;
	float errorTh1 = 0.7 * noiseStdDev;
	float errorTh2 = 0.7 * 0.7 * noiseStdDev;

	if(error > errorTh1)
	{
		float e = error - errorTh1;
		
		if( decTime == 0 && incTime ==0 )
		{
			newDev = e; 
		}
		else
		{
			newDev *= pow(2, incTime+1);
			newDev = newDev > 8 * noiseStdDev ? noiseStdDev : newDev;
		}
		
		incTime++;
		decTime = 0;
		KF->measurementNoiseCov = *( Mat_<float>(2,2) << newDev,0, 0,newDev );
		cout << "kalmanDynamicAdjust: error is enlarging ! e: " << e << "  newDev: " << newDev <<endl;

	}
	
	if( error < errorTh1 && error > errorTh2 )
	{
		if( newDev > defaultMeasureDev ) 
		{
			decTime++;
			incTime = 0;
			newDev *= pow(2, incTime+1);
		}
		
		newDev = newDev < defaultMeasureDev ? defaultMeasureDev : newDev;
	
		KF->measurementNoiseCov = *( Mat_<float>(2,2) << newDev,0, 0,newDev );
		cout << "kalmanDynamicAdjust: error is decreasing !  newDev: " << newDev <<endl;
	}
}

/**
  Real implementation for speed adaptive KF
  @param measurement: noised dataset 
*/
Point SpeedAdaptiveKalmanFilter::filter( Mat_<float> measurement )
{
	Point last1, last2;
	
	if(first)
	{
		KF->statePost.at<float>(0) = measurement(0);
		KF->statePost.at<float>(1) = measurement(1);
		KF->statePost.at<float>(2) = 0;
		KF->statePost.at<float>(3) = 0;
		first = 0;
	}
	
	KF->predict();
	Mat estimated = KF->correct(measurement);
	
	cout << " After correction: " <<endl;
	cout << KF->statePost.at<float>(0) << " " << KF->statePost.at<float>(1) << " " << KF->statePost.at<float>(2) << " " << KF->statePost.at<float>(3) << endl;
	cout << "Speed: " << sqrt( pow(KF->statePost.at<float>(2),2) + pow(KF->statePost.at<float>(3),2) ) << endl;
	
	Point ptEstimated( estimated.at<float>(0), estimated.at<float>(1) );
	kalmanCorrect->push_back(ptEstimated);  
	
	if( kalmanCorrect->size() > 2 )
	{
		last1 = kalmanCorrect->at( kalmanCorrect->size() - 2 );
		last2 = kalmanCorrect->at( kalmanCorrect->size() - 3 );
		
		int len0 = sqrt( pow( last1.x -  ptEstimated.x, 2) + pow( last1.y -  ptEstimated.y, 2) );
		int len1 = sqrt( pow( last1.x -  last2.x, 2) + pow( last1.y -  last2.y, 2) );
		
		int xInc = abs( last1.x -  ptEstimated.x );
		int yInc = abs( last1.y -  ptEstimated.y );
		
		if(0) //swwitch off
		{
			if( ( len0 > len1 && len0 > noiseStdDev ) ||  //speed up
			    ( len1 > len0 && len1 > noiseStdDev ) )   //speed down
			{
				cout << " ******** Speed UP/DOWN is IN ********** " <<endl;
				if( abs( last1.x - ptEstimated.x ) > abs( last1.y - ptEstimated.y ) )
				{
					speedXDev += .2;
				}
				else
				{
					speedYDev += .2;
				}
			}
			else
			{
				speedXDev = .1;
				speedYDev = .1;
			}
			
			cout << " speedXDev: " << speedXDev << " speedYDev " << speedYDev <<endl;
			KF->processNoiseCov = *(Mat_<float>(4, 4) << .1,0,0,0,  0,.1,0,0, 0,0,speedXDev,0, 0,0,0,speedYDev );
		}
		
		if(1)
		{
			if( len0 - len1 > 10 && len0 > noiseStdDev )  //speed up
			{
				cout << " ********* Speed Up ************ " <<endl;
				KF->statePost.at<float>(2) = 1.1 * KF->statePost.at<float>(2);
				KF->statePost.at<float>(3) = 1.1 * KF->statePost.at<float>(3);
			}
			else if( len1 - len0 > 10 && len1 > noiseStdDev )  //speed down
			{
				cout << " ********* Speed Down ************ " <<endl;
				KF->statePost.at<float>(2) = .1 * KF->statePost.at<float>(2);
				KF->statePost.at<float>(3) = .1 * KF->statePost.at<float>(3);			
			
			}
		}
		else
		{
			int lenInc = abs(len0 - len1);
			int directInc = xInc - yInc;
			int dynamic = 0; //swither
		
			if( len0 - len1 > 10 && len0 > noiseStdDev )  // speed up
			{
				if( directInc >= 10 )
				{
					if( dynamic )
					{
						KF->statePost.at<float>(2) = pow( 1.2, 1 + lenInc/maxMove + directInc/maxSpeed ) * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = pow( 1.2, 1 + lenInc/maxMove + directInc/maxSpeed ) * KF->statePost.at<float>(3);	
						cout << "Speed Up --- Dynamic  x > y : " << KF->statePost.at<float>(2) << "  " << KF->statePost.at<float>(3) <<endl;
					}
					else
					{
						KF->statePost.at<float>(2) = 1.5 * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = 1.1 * KF->statePost.at<float>(3);
						cout << " Speed Up --- Static   x >y " << endl;
					}

				}
				else if( directInc <= -10 )
				{
					if( dynamic )
					{
						//KF.statePost.at<float>(2) = pow( 1.2, 1 + lenInc/maxMove + directInc/maxSpeed ) * KF.statePost.at<float>(2);
						KF->statePost.at<float>(2) = pow( 1.2, 1 + lenInc/maxMove ) * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = pow( 1.2, 1 + lenInc/maxMove + directInc/maxSpeed ) * KF->statePost.at<float>(3);	
						cout << "Speed Up --- Dynamic  y > x : " << KF->statePost.at<float>(2) << "  " << KF->statePost.at<float>(3) <<endl;
					}
					else
					{
						KF->statePost.at<float>(2) = 1.1 * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = 1.5 * KF->statePost.at<float>(3);
						cout << " Speed Up --- Static   y > x " << endl;
					}
				}
				else
				{
					KF->statePost.at<float>(2) = 1.1 * KF->statePost.at<float>(2);
					KF->statePost.at<float>(3) = 1.1 * KF->statePost.at<float>(3);
					cout << " Speed Up --- in 10 interval --- 1.1 " << endl;				
				}
			}
			else if( len1 - len0 > 10 && len1 > noiseStdDev )  // speed down
			{
				if( directInc >= 10 )
				{
					if( dynamic )
					{
						KF->statePost.at<float>(2) = pow( 1.2, 1 - lenInc/maxMove + directInc/maxSpeed ) * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = pow( 1.2, 1 - lenInc/maxMove  ) * KF->statePost.at<float>(3);	
						cout << "Speed Down --- Dynamic  x > y : " << KF->statePost.at<float>(2) << "  " << KF->statePost.at<float>(3) <<endl;
					}
					else
					{
						KF->statePost.at<float>(2) = .5 * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = .3 * KF->statePost.at<float>(3);
						cout << " Speed Down --- Static   x >y " << endl;
					}
				}
				else if( directInc <= -10 )
				{
					if( dynamic )
					{
					
						KF->statePost.at<float>(2) = pow( 1.2, 1 - lenInc/maxMove ) * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = pow( 1.2, 1 - lenInc/maxMove + directInc/maxSpeed ) * KF->statePost.at<float>(3);	
						cout << "Speed Down --- Dynamic  y > x : " << KF->statePost.at<float>(2) << "  " << KF->statePost.at<float>(3) <<endl;
					}
					else
					{
						KF->statePost.at<float>(2) = .3 * KF->statePost.at<float>(2);
						KF->statePost.at<float>(3) = .5 * KF->statePost.at<float>(3);
						cout << " Speed Down --- Static   y > x " << endl;
					}
				}
				else
				{
					KF->statePost.at<float>(2) = .3 * KF->statePost.at<float>(2);
					KF->statePost.at<float>(3) = .3 * KF->statePost.at<float>(3);
					cout << " Speed Down --- in 10 interval --- 0.3 " << endl;				
				}
			}
		}
	}

	float error_noised_estimate = sqrt( pow( measurement(0) - ptEstimated.x, 2 ) + pow( measurement(1) - ptEstimated.y, 2 ) );

    if(1) //swith on
	{
		kalmanDynamicAdjust( error_noised_estimate );
	}
	
	cout << endl << endl;
	return ptEstimated;
}
