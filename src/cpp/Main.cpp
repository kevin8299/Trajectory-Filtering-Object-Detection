/**
  Main entry function for this Trajectory Filtering & Object Detection
*/
#include "stdafx.h"
#include "Main.h"

/**
  Instruction to guide user for next operations
  @param: None
*/
void showInfoMain() {

	cout << "============================================================" <<endl;
	cout << "Choose which program you want to be in: " << endl;
	cout << "  1  ----> Trajectory Filtering Simulation" << endl;
	cout << "  2  ----> Video Object Detection Program" << endl;
	cout << " 'c' ----> Type 'c' to EXIT" << endl;
	cout << "============================================================" << endl;

}

/**
  Main function to enter the program
  @param: None
*/
void main(int argc, char* const argv[])
{
	showInfoMain();

	int code = (char) -1;
	while( true )
	{
		code = getchar();
		if( code == '1' ) {
			cout << "\nTrajectory Filtering Simulation is Beginning : \n" << endl;
			trajectoryFiltering();
			cout << "\nTrajectory Filtering Simulation is OVER \n" << endl;
			showInfoMain();
		}
		else if( code == '2' ) {
			cout << "\nObject Detection is Beginning : \n" << endl;
		    objDetection();
			cout << "\nObject Detection is OVER \n" << endl;
			showInfoMain();
		}
		else if( code == 'c' )
			break;
	}
}