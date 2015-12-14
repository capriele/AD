/* 
 * Author: Guivenca
 */

#include "../pgr2opencv.cpp"
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;
/*
 * 
 */
bool contour_compare (vector<Point> i,vector<Point>j) { return (contourArea(i,false)>contourArea(j,false)); }

int main(int argc, char** argv) {
	struct timeval tp;
	gettimeofday(&tp,NULL);
	long int timestamp=tp.tv_sec;
	Size S=Size(1600,1200); // change accordingly
	VideoWriter outputVideo;
	int ex=-1;
	
 	outputVideo.open("teste.mpg", CV_FOURCC('X','2','6','5'), 20, S, true);
 	if (!outputVideo.isOpened())
    	{
        	cout  << "Could not open the output video for write: " << endl;
        	return -1;
    	}
    	


	Mat gray_src;
	Mat gray_colored;
    pgr2cv::VideoCapture cap;
    cap.open(0);
    while(waitKey(5)<=0){ 
	    cap>>gray_src;
	    cvtColor(gray_src,gray_colored,CV_GRAY2RGB);
	    outputVideo<<gray_colored;
	    namedWindow( "view", CV_WINDOW_NORMAL );
	    imshow("view",gray_src);
	    resizeWindow("view",1600,1200);
  }
    return 0;
}

