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

	// Notes to self
	// cvGrabFrame captures a frame
	// cvRetrieveFrame retrieves the captured frame
	
	IplImage* SAMPLE = 0; 
	int SAMPLE_HEIGHT,SAMPLE_WIDTH;
	int CAMERA_WARM_UP_TIME = 3; // Seconds, not milliseconds
	int CAMERA_DEVICE_NUMBER = 0; // /dev/video0
	int CAMERA_WIDTH_RESOLUTION = 1280;
	int CAMERA_HEIGHT_RESOLUTION = 800;

	// Can't seem to supress the error message, so tell
	// the user it's safe to ignore...
	cout << "It is safe to ignore the 'VIDIOC_QUERYMENU: Invalid argument' warnings...\n";
		
	// Initialize capture from camera
	CvCapture* CAMERA = cvCaptureFromCAM(CAMERA_DEVICE_NUMBER);

	//Set the Camera Resolution
	cvSetCaptureProperty(CAMERA,CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH_RESOLUTION);
	cvSetCaptureProperty(CAMERA,CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT_RESOLUTION);

	// Wait for the camera to start up / focus
	sleep(CAMERA_WARM_UP_TIME);

	// Make sure we are capturing at the right resolution
	cvQueryFrame(CAMERA);
	int FRAME_HEIGHT = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_HEIGHT);
	int FRAME_WIDTH = (int) cvGetCaptureProperty(CAMERA, CV_CAP_PROP_FRAME_WIDTH);
	cout << "Capturing at " << FRAME_WIDTH << "x" << FRAME_HEIGHT << "\n";
	// Will likely need to adjust resolution... Add later.

	// Record a sample
	cvGrabFrame(CAMERA);
	SAMPLE = cvRetrieveFrame(CAMERA);

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", SAMPLE);
	cvWaitKey(0);

	// Lets save an image
	cvSaveImage("test.bmp",SAMPLE);

	// Convert it to Grey ; will use this later
	IplImage* GREY_SAMPLE = cvCreateImage(cvSize(FRAME_WIDTH,FRAME_HEIGHT),IPL_DEPTH_8U,1);
	cvCvtColor(SAMPLE,GREY_SAMPLE,CV_RGB2GRAY);

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", GREY_SAMPLE);
	cvWaitKey(0);

	///////////////////////////////////////
	// Lets trying recording a movie just for fun...
	// It might be advantagious to record a movie and use hat as the "samples"...
     	int isColor = 1;
	int fps = 30;
	int frameW = 1280;
	int frameH = 800;
	int nFrames = 120;
	//Initialize a writer
	CvVideoWriter *writer = 0;
	writer = cvCreateVideoWriter("out.avi",CV_FOURCC('P','I','M','1'),fps,cvSize(frameW,frameH),isColor);
	//Write the video file
	//IplImage* img = 0; 
	for (int i=0; i < nFrames; i++) {
		cvGrabFrame(CAMERA);
		SAMPLE=cvRetrieveFrame(CAMERA);
		cvWriteFrame(writer,SAMPLE);
	}
	// Release the writer
	cvReleaseVideoWriter(&writer);
	/////////////////////////////////
	  
	// Draw our calibration line to make sure its right
	cvLine(SAMPLE, cvPoint(357,535), cvPoint(883,450), cvScalar(0,255,0), 1);
	//Add in our equation later...

	// Lets see the image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 100, 100);
	cvShowImage("mainWin", SAMPLE);
	cvWaitKey(0);

	// Lets save an image
	cvSaveImage("test-line.bmp",SAMPLE);

	// Turn the camera off
	cvReleaseCapture(&CAMERA);
	return 0;
}
