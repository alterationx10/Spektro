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
	y = (0.161597*x - 592.69)*-1;
	return(y);
}


int main(int argc, char *argv[]) {

	// Notes to self
	// cvGrabFrame captures a frame
	// cvRetrieveFrame retrieves the captured frame

	// Ge the Number of Samples to take from the command line
	int NSAMPLES=0;
	NSAMPLES = atoi(argv[1]);

	IplImage* SAMPLE = 0; 
	int SAMPLE_HEIGHT,SAMPLE_WIDTH;
	int CAMERA_WARM_UP_TIME = 3; // Seconds, not milliseconds
	int CAMERA_DEVICE_NUMBER = 0; // /dev/video0
	int CAMERA_WIDTH_RESOLUTION = 1280;
	int CAMERA_HEIGHT_RESOLUTION = 800;
	int RECORD_MOVIE = 0; // 0/1 False/True
	double AVERAGE[770];
	double AVERAGE_B[770];
	double AVERAGE_G[770];
	double AVERAGE_R[770];

	// Zero the AVERAGE array
	for (int i=0; i<771; i++) {
		AVERAGE[i] = 0;
		AVERAGE_B[i] = 0;
		AVERAGE_G[i] = 0;
		AVERAGE_R[i] = 0;
	}

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
	//cvGrabFrame(CAMERA);
	//SAMPLE = cvRetrieveFrame(CAMERA);

	// Lets see the image
	//cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	//cvMoveWindow("mainWin", 100, 100);
	//cvShowImage("mainWin", SAMPLE);
	//cvWaitKey(0);

	// Lets save an image
	//cvSaveImage("test.bmp",SAMPLE);

	// Convert it to Grey ; will use this later
	//IplImage* GREY_SAMPLE = cvCreateImage(cvSize(FRAME_WIDTH,FRAME_HEIGHT),IPL_DEPTH_8U,1);
	//cvCvtColor(SAMPLE,GREY_SAMPLE,CV_RGB2GRAY);

	// Lets see the image
	//cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	//cvMoveWindow("mainWin", 100, 100);
	//cvShowImage("mainWin", GREY_SAMPLE);
	//cvWaitKey(0);

	///////////////////////////////////////
	// Lets trying recording a movie just for fun...
	if (RECORD_MOVIE) {
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
	}
	/////////////////////////////////
	  
	// Draw our calibration line to make sure its right
	//cvLine(SAMPLE, cvPoint(357,535), cvPoint(883,450), cvScalar(0,255,0), 1); //Main line
	//cvLine(SAMPLE, cvPoint(420,515), cvPoint(627,477), cvScalar(0,255,0), 1); //Second line
	//Add in our equation later...

	// Lets see the image
	//cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	//cvMoveWindow("mainWin", 100, 100);
	//cvShowImage("mainWin", SAMPLE);
	//cvWaitKey(0);

	// Lets save an image
	//cvSaveImage("test-line.bmp",GREY_SAMPLE);

	//Try spitting out the "RGB" value
	//CvScalar s;
	//cout << s.val[0] << "\n";

	// Lets try doing a loop
	//IplImage* GREY_SAMPLE = cvCreateImage(cvSize(FRAME_WIDTH,FRAME_HEIGHT),IPL_DEPTH_8U,1);
	CvScalar pixel;
	cout << "We are using " << NSAMPLES << " images \n";
  	for (int i=0; i<NSAMPLES; i++) {
  		cvGrabFrame(CAMERA);
  		SAMPLE=cvRetrieveFrame(CAMERA);
		//cvCvtColor(SAMPLE,GREY_SAMPLE,CV_RGB2GRAY);
		// We want to pick x,y values for from 230 to 1000 
		for (int x=230; x <1001; x++) {
			// What y value do we need?
			int y;
			y = main_line(x);
			// Get the pixel vale
			pixel=cvGet2D(SAMPLE,y,x);
			// Sum it up in an array
			AVERAGE_B[x-230] = AVERAGE_B[x-230] + pixel.val[0];
			AVERAGE_G[x-230] = AVERAGE_G[x-230] + pixel.val[1];
			AVERAGE_R[x-230] = AVERAGE_R[x-230] + pixel.val[2];
			AVERAGE[x-230] = AVERAGE[x-230] + (AVERAGE_B[x-230] + AVERAGE_G[x-230] + AVERAGE_R[x-230])/3;
			// Move on to next image :-)
		}
  	}
	// Divide by number of samples for averaging
  	for (int i=0; i<771; i++) {
		//AVERAGE_B[i] = AVERAGE_B[i] / NSAMPLES ;
		//AVERAGE_G[i] = AVERAGE_G[i] / NSAMPLES ;
		//AVERAGE_R[i] = AVERAGE_R[i] / NSAMPLES ;
		AVERAGE[i] = AVERAGE[i] / NSAMPLES ;
		// spit it out while we're at it
		//cout << i << " " << AVERAGE_B[i] << " " << AVERAGE_G[i] << " " << AVERAGE_R[i] << "\n";
		cout << i << " " << AVERAGE[i] << "\n";
	}


	// Turn the camera off
	cvReleaseCapture(&CAMERA);
	return 0;
}
