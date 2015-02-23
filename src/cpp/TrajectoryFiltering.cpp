/**
  Main function for Trajectory Filtering
*/
#include "stdafx.h"
#include "TrajectoryFiltering.h"

/**
  Instruction to guide user for next operations
  @param: None
*/
void showInfoFilter() {

	cout << "=================================================================" <<endl;
	cout << "Choose which mode you want to be in: " << endl;
	cout << "  1 ----> Use Mouse Input for Online Kalman Filtering Debug" << endl;
	cout << "  2 ----> Use Stored Dataset for Offline Kalman Filtering Debug" << endl;
	cout << "  3 ----> Use Mouse to Create Dataset" << endl;
	cout << "  4 ----> Use Stored Dataset for Offline NN Filtering Debug" << endl;
	cout << "  5 ----> Use Mouse Input for Online LMS Filtering Debug" << endl;
	cout << " 'c' ----> Type 'c' to EXIT" << endl;
	cout << "=================================================================" <<endl;

}

/**
  Main function for Trajectory Filtering
  @param: None
*/
void trajectoryFiltering(){

	showInfoFilter();

	Simulator s;
	char code = ( char ) -1;

	while( true )
	{
		code = getchar();
		if( code == '1' )  //mouse input online debug
		{
			cout << "Realtime Filtering with KF is Beginning: " << endl;
			s.realtimeFiltering();
			cout << "Realtime Filtering with KF is OVER" << endl;
			showInfoFilter();
		}
		else if( code == '2' )  //use stored dataset for offline debug
		{
			Static_Analysis:
				cout << "StoredData Filtering with KF is Beginning: " << endl;
				s.dataReadShow();
				s.storedDataKFFiltering();
				cout << "StoredData Filtering with KF is OVER" << endl;
		
				cout << "Press 's' to show Filtering Result or 'c' to EXIT to Main Trajectory Filtering " << endl;
				
				int c = -1 ;
				while(true){
					c = getchar();
					if( c == 's' ) 
					{
						s.showResult();
						showInfoFilter();
						break;
					}
					else if( c == 'c' ) {
						showInfoFilter();
						break;
					}
				}
		}
		else if( code == '3' )  //Create dataset
		{		
			cout << "Data Creation is Beginning: " << endl;
			s.createData("datasetClear.txt", "datasetNoised.txt");
			cout << "Data Creation is OVER" << endl;

			cout << "Press 'n' to continue StoredData Filtering or 'c' to EXIT to Main Trajectory Filtering " << endl;
			
			int c = -1;
			while(true){
				c = getchar();			
				if( c == 'n' ) 
					goto Static_Analysis;
				else if( c== 'c' ) {
					showInfoFilter();
					break;
				}
			}
		}
		else if( code == '4' )  //Neural Network Filtering 
		{	
			cout << "Neural Network Filtering with KF is Beginning: " << endl;
			s.nnFiltering();
			cout << "Neural Network Filtering with KF is OVER\n" << endl;
			showInfoFilter();
		}
		else if( code == '5' )  //LMS
		{
			cout << "LMS Filtering with KF is Beginning: " << endl;
			s.lmsFiltering();
			cout << "LMS Filtering with KF is OVER\n" << endl;
			showInfoFilter();
		}	
		else if( code == 'c' )  //use stored dataset for offline debug
		{
			cout << "Back to TrajectoryFiltering & Object Detection Menu Now !" << endl;
		    break;
		}
	}

}




