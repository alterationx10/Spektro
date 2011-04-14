// Spektro
// A C++ program to run my specrometer

// OpenCV includes
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

// Misc. includes
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;
#include <string>

int main_line (int x) {
	int y;
	//y = (0.161597*x - 592.69)*-1;
	y = (0.172144*x - 590.512)*-1;
	return(y);
	//This needs work...
}

// Functions for easy IN/OUT ///////////////////////////////////

// Clear the screen (LINUX)
void clear() {
	system("clear");
}

// Wait for a Return
void wfr() {
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

// Wait For Return AND clear
void wfrc(){
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	system("clear");
}

// Easily print messages to user
void echo(string info){
	cout <<  info  << endl;
}
////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	// Variable Setup /////////////////////////////////////////////////////////////////////////
	// Camera variables
	IplImage* SAMPLE = 0; 			// The image variable we will work with
	int CAMERA_WARM_UP_TIME = 3; 		// Seconds, not milliseconds
	int CAMERA_DEVICE_NUMBER = 0; 		// /dev/video#
	int CAMERA_WIDTH_RESOLUTION = 1280;
	int CAMERA_HEIGHT_RESOLUTION = 800;


	// Camera logical
	// 0/1 False/True
	int RECORD_MOVIE = 0; 
	int TAKE_PICTURE = 0;
	int RECORD_CALIBRATION = 0;
	int ACQUIRE_DATA = 1;
	int SMOOTH_DATA = 0; // Will add later
	
	// Set up an array to hold our data
	//double NOISE[770];
	double BLANK[770];
	double SAMPLE_DATA[770];
	// Zero the AVERAGE array
	for (int i=0; i<771; i++) {
		BLANK[i] 	= 0;
		SAMPLE_DATA[i] 	= 0;
	}
	///////////////////////////////////////////////////////////////////////////////////

	// Parse command line options
	//
	// Help the user out
	if (0) {
	if (argc = 1) {
	       cout << "Program options are:\n";
	       cout << "run=# (of samples to average)\n";
	       cout << "movie=1 (to output a 5 sec. video: sample.avi)\n";
	       cout << "picture=1 (to output a picture: sample.bmp)\n";
	       cout << "calibrate=1 (to output a picture with the calibration line drawn on it)\n)";
	       cout << "smooth=1 (to run a data averaging algorithm on the data to smooth it out);\n)";
	       exit(0);
	}
	}

       			       
	int NSAMPLES=0;
	NSAMPLES = atoi(argv[1]);
	// Will add more options later so user can toggle the above logical values



	// Initialize the Camera /////////////////////////////////////////////////////////////////////////
	//												//
	// Can't seem to supress the error message, so tell the user it's safe to ignore...		//
	cout << "# It is safe to ignore the 'VIDIOC_QUERYMENU: Invalid argument' warnings...\n";	//
	//												//	
	// Initialize capture from camera								//
	CvCapture* CAMERA = cvCaptureFromCAM(CAMERA_DEVICE_NUMBER);					//
	//												//	
	//Set the Camera Resolution									//
	cvSetCaptureProperty(CAMERA,CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH_RESOLUTION);			//
	cvSetCaptureProperty(CAMERA,CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT_RESOLUTION);		//
	//												//	
	// Wait for the camera to start up / focus							//
	sleep(CAMERA_WARM_UP_TIME);									//
	//												//	
	// Make sure we are capturing at the right resolution						//
	cvQueryFrame(CAMERA);										//
	int FRAME_HEIGHT = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_HEIGHT);		//
	int FRAME_WIDTH = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_WIDTH);			//
	cout << "# Capturing at " << FRAME_WIDTH << "x" << FRAME_HEIGHT << "\n";			//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	
	// Take a Picture ////////////////////////////////
	if (TAKE_PICTURE) {				//
	cvGrabFrame(CAMERA);				//
	SAMPLE = cvRetrieveFrame(CAMERA);		//
	//						//
	// Show the image				//
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);	//
	cvMoveWindow("mainWin", 100, 100);		//
	cvShowImage("mainWin", SAMPLE);			//
	// Wait for the user to press a key		//
	cvWaitKey(0);					//
	//						//
	// Save the image				//
	cvSaveImage("SAMPLE.bmp",SAMPLE);		//
	}						//
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Record a movie											//
	if (RECORD_MOVIE) {											//
     	int isColor = 1;											//
	int fps = 30;												//
	int frameW = 1280;											//
	int frameH = 800;											//
	int movie_length = 5;											//
	int nFrames = fps*movie_length;										//
	//Initialize a writer											//
	CvVideoWriter *writer = 0;										//
	writer = cvCreateVideoWriter("sample.avi",CV_FOURCC('P','I','M','1'),fps,cvSize(frameW,frameH),isColor);//
	//Write the video file											//
	for (int i=0; i < nFrames; i++) {									//
		cvGrabFrame(CAMERA);										//
		SAMPLE=cvRetrieveFrame(CAMERA);									//
		cvWriteFrame(writer,SAMPLE);									//
	}													//
	// Release the writer											//
	cvReleaseVideoWriter(&writer);										//
	}													//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	// Show calibration data /////////////////////////////////
	if (RECORD_CALIBRATION)	{				//
		CvScalar CALIBRATION;				//
		// Take a picture				//
  		cvGrabFrame(CAMERA);				//
  		SAMPLE=cvRetrieveFrame(CAMERA);			//
		// Go over the same loop as in acquire data	//
		for (int x=230; x <1001; x++) {			//
			int c_y;				//
			c_y = main_line(x);			//
			CALIBRATION = cvGet2D(SAMPLE,c_y,x);	//
			// An orange color: 0,69,255		//
			CALIBRATION.val[0] = 0;			//
			CALIBRATION.val[1] = 69;		//
			CALIBRATION.val[2] = 255;		//
			cvSet2D(SAMPLE,c_y,x,CALIBRATION);	//
		}						//
		// Save the image				//
		cvSaveImage("calibration.bmp",SAMPLE);		//
	}							//
	//////////////////////////////////////////////////////////
		


	// Measure the blank
	clear();
	echo("# Ensure that the lamp is ON");
	echo("# Place in the BLANK");
	wfr();
	// Acquire data //////////////////////////////////////////////////////////////////////////////////
	if (ACQUIRE_DATA) {										//
	CvScalar pixel;											//
	cout << "# We are using " << NSAMPLES << " images \n";						//
  	for (int i=0; i<NSAMPLES; i++) {								//
  		cvGrabFrame(CAMERA);									//
  		SAMPLE=cvRetrieveFrame(CAMERA);								//
		// We want to pick x,y values for from x = 230 to 1000 					//
		for (int x=230; x <1001; x++) {								//
			// What y value do we need?							//
			int y;										//
			y = main_line(x);								//
			// Get the pixel value								//
			pixel=cvGet2D(SAMPLE,y,x);							//
			// Sum it up in an array							//
			BLANK[x-230] = BLANK[x-230] + pixel.val[0] + pixel.val[1] + pixel.val[2] ; 	//
		}											//
  	}												//
	//												//
	// Divide by number of samples for averaging							//
  	for (int i=0; i<771; i++) {									//
		BLANK[i] = BLANK[i] / (double) NSAMPLES ;						//
	}												//
	}												//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// Measure the sample
	clear();
	echo("# Remove the blank and place in the SAMPLE");
	wfr();
	// Acquire data //////////////////////////////////////////////////////////////////////////////////
	if (ACQUIRE_DATA) {										//
	CvScalar pixel;											//
	cout << "# We are using " << NSAMPLES << " images \n";						//
  	for (int i=0; i<NSAMPLES; i++) {								//
  		cvGrabFrame(CAMERA);									//
  		SAMPLE=cvRetrieveFrame(CAMERA);								//
		// We want to pick x,y values for from x = 230 to 1000 					//
		for (int x=230; x <1001; x++) {								//
			// What y value do we need?							//
			int y;										//
			y = main_line(x);								//
			// Get the pixel value								//
			pixel=cvGet2D(SAMPLE,y,x);							//
			// Sum it up in an array							//
			SAMPLE_DATA[x-230] = SAMPLE_DATA[x-230] 					//
							+ pixel.val[0] + pixel.val[1] + pixel.val[2] ; 	//
		}											//
  	}												//
	//												//
	// Divide by number of samples for averaging							//
  	for (int i=0; i<771; i++) {									//
		SAMPLE_DATA[i] = SAMPLE_DATA[i] / (double) NSAMPLES ;					//
	}												//
	}												//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// Turn the camera off
	cvReleaseCapture(&CAMERA);

	// Data Processing 

	// Calculate the absorbance
	for (int i=0; i<771; i++) {
		cout << i << "\t" << log( BLANK[i] / SAMPLE_DATA[i] ) << "\n";
	}

	// Finished
	return 0;
}
