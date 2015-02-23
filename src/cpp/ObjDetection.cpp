/**
  Main entry function for Object Detection
*/
#include "stdafx.h"
#include "ObjDetection.h"

/**
  Instruction to guide user for next operations
  @param: None
*/
void showInfoDetect(){

	cout << "=======================================================================" <<endl;
	cout << "Choose which mode you want to be in: " << endl;
	cout << "  1 ----> Use Background Subtraction with GMM" << endl;
	cout << "  2 ----> Use Mixture Algorithm of Color Analysis and Object Detection" << endl;
	cout << " 'c' ----> Type 'c' to EXIT" << endl;
	cout << "=======================================================================" << endl;

}

/**
  Main function for Object Detection
  @param: None
*/
void objDetection()
{
	showInfoDetect();

	char code = ( char ) -1;
	ObjDetector od;

	while( true )
	{
		code = getchar();
		VideoCapture video("test.mp4");
		if( code == '1' ) {
			cout << "\nBackground Subtraction with GMM Simulation is Beginning : \n" << endl;
			od.bgGMM(video);	
			cout << "\nBackground Subtraction with GMM Simulation is OVER\n" << endl;
			showInfoDetect();
		}
		else if( code == '2' ) {
			cout << "\nMixture Algorithm of Color Analysis and Object Detection Simulation is Beginning : \n" << endl;
			od.colorAnalysis_decisionTree(video);
			cout << "\nMixture Algorithm of Color Analysis and Object Detection Simulation is OVER\n" << endl;
			showInfoDetect();
		}
		else if( code == 'c' ) {
			cout << "Back to TrajectoryFiltering & Object Detection Menu Now !" << endl;
			break;
		}
	}
}
