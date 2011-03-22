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

int main(int argc, char *argv[]) {

	IplImage* SAMPLE = 0; 
	IplImage* GREY_SAMPLE = 0; 
	int SAMPLE_HEIGHT,SAMPLE_WIDTH;
	int CAMERA_WARM_UP_TIME = 3000;
	int CAMERA_DEVICE_NUMBER = 0; // /dev/video0


	// Initialize capture from camera
	CvCapture* CAMERA = cvCaptureFromCAM(CAMERA_DEVICE_NUMBER);

	// Wait for the camera to start up / focus
	sleep(CAMERA_WARM_UP_TIME);

	// Make sure we are capturing at the right resolution
	cvQueryFrame(CAMERA);
	int FRAME_HEIGHT = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_HEIGHT);
	int FRAME_WIDTH = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_WIDTH);
	cout << "Capturing at " << FRAME_WIDTH << "x" << FRAME_HEIGHT << "\n";
	// Will likely need to adjust resolution... Add later.

	// Record a sample
	SAMPLE = cvRetrieveFrame(CAMERA);

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", SAMPLE);
	cvWaitKey(0);
	
	// Convert it to Grey
	cvCvtColor(SAMPLE,GREY_SAMPLE,CV_RGB2GRAY);	

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", GREY_SAMPLE);
	cvWaitKey(0);

	// Draw our calibration line to make sure its right
	cvLine(GREY_SAMPLE, cvPoint(100,100), cvPoint(200,200), cvScalar(0,255,0), 1);
	//Add in our equation later...

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", GREY_SAMPLE);
	cvWaitKey(0);


	

	// Turn the camera off
	cvReleaseCapture(&CAMERA);
	return 0;
}
