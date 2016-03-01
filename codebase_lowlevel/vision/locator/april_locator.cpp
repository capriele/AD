/*
@file april_locator.cpp
@brief Simple april tag locator
@author: Thomas Sayre-McCord
@date: 02/18/2016

Based off apriltags_demo.cpp written by Michael Kaess. 

Opens the first available camera and continuously detects April 
tags in the incoming images. Determines position of the camera from
known positions of tags and uses least-squares type approximation of 
camera location
*/

/*
  TODO:
  * Concern about processing lag -- prefer to stay up to date with camera than process all frames
  * Better matching for fisheye lens
  * If faster processing is needed can move from april tags to simpler objects
  * Estimating covariance? -- look at iSAM code
*/

using namespace std;


// OpenCV library for easy access to USB camera and drawing of images
// on screen
#include "opencv2/opencv.hpp"

// Point grey camera interface
#include "../pgr2opencv.cpp"

// April tags detector and tag family
#include "AprilTags/TagDetector.h"
#include "AprilTags/Tag16h5.h"

#include <cmath>  // Used for standardRad

#ifndef PI
const double PI = 3.14159265358979323846;
#endif
const double TWOPI = 2.0*PI;

/**
 * Normalize angle to be within the interval [-pi,pi].
 */
inline double standardRad(double t) {
    if (t >= 0.) {
	t = fmod(t+PI, TWOPI) - PI;
    } else {
	t = fmod(t-PI, -TWOPI) + PI;
    }
    return t;
}

/**
 * Convert rotation matrix to Euler angles
 */
void wRo_to_euler(const Eigen::Matrix3d& wRo, double& yaw, double& pitch, double& roll) {
    yaw = standardRad(atan2(wRo(1,0), wRo(0,0)));
    double c = cos(yaw);
    double s = sin(yaw);
    pitch = standardRad(atan2(-wRo(2,0), wRo(0,0)*c + wRo(1,0)*s));
    roll  = standardRad(atan2(wRo(0,2)*s - wRo(1,2)*c, -wRo(0,1)*s + wRo(1,1)*c));
}


// Main processing class

class Detect
{

    AprilTags::TagDetector* m_tagDetector;
    AprilTags::TagCodes m_tagCodes;

    bool m_draw; // draw image and April tag detections?
    bool m_arduino; // send tag detections to serial port?
    bool m_timing; // print timing information for each tag extraction call

    int m_width; // image size in pixels
    int m_height;
    double m_tagSize; // April tag side length in meters of square black frame
    double m_fx; // camera focal length in pixels
    double m_fy;
    double m_px; // camera principal point
    double m_py;

    int m_deviceId; // camera id (in case of multiple cameras)

    // cv::VideoCapture m_cap;
    pgr2cv::VideoCapture cap;
   
public:

  // default constructor
    Detect() :
	// default settings, most can be modified through command line options (see below)
	m_tagDetector(NULL),
	m_tagCodes(AprilTags::tagCodes36h11),

	m_timing(false),

	m_width(640),
	m_height(480),
	m_tagSize(0.166),
	m_fx(600),
	m_fy(600),
	m_px(m_width/2),
	m_py(m_height/2),

	m_deviceId(0)
    {}

  // changing the tag family
    void setTagCodes(string s) {
	if (s=="16h5") {
	    m_tagCodes = AprilTags::tagCodes16h5;
	} else if (s=="25h7") {
	    m_tagCodes = AprilTags::tagCodes25h7;
	} else if (s=="25h9") {
	    m_tagCodes = AprilTags::tagCodes25h9;
	} else if (s=="36h9") {
	    m_tagCodes = AprilTags::tagCodes36h9;
	} else if (s=="36h11") {
	    m_tagCodes = AprilTags::tagCodes36h11;
	} else {
	    cout << "Invalid tag family specified" << endl;
	    exit(1);
	}
    }

  // parse command line options to change default behavior
    void parseOptions(int argc, char* argv[]) {
	int c;
	while ((c = getopt(argc, argv, ":h?adtC:F:H:S:W:E:G:B:D:")) != -1) {
	    // Each option character has to be in the string in getopt();
	    // the first colon changes the error character from '?' to ':';
	    // a colon after an option means that there is an extra
	    // parameter to this option; 'W' is a reserved character
	    switch (c) {
	    case 'h':
	    case '?':
		cout << intro;
		cout << usage;
		exit(0);
		break;
	    case 't':
		m_timing = true;
		break;
	    case 'C':
		setTagCodes(optarg);
		break;
	    case 'F':
		m_fx = atof(optarg);
		m_fy = m_fx;
		break;
	    case 'H':
		m_height = atoi(optarg);
		m_py = m_height/2;
		break;
	    case 'S':
		m_tagSize = atof(optarg);
		break;
	    case 'W':
		m_width = atoi(optarg);
		m_px = m_width/2;
		break;
	    case 'D':
		m_deviceId = atoi(optarg);
		break;
	    case ':': // unknown option, from getopt
		cout << intro;
		cout << usage;
		exit(1);
		break;
	    }
	}

    }

    void setup() {
	m_tagDetector = new AprilTags::TagDetector(m_tagCodes);

	// find and open a USB camera
	// m_cap =  cv::VideoCapture(m_deviceId);
        // if(!m_cap.isOpened()) {
	//     cerr << "ERROR: Can't find video device " << m_deviceId << "\n";
	//     exit(1);
	// }
	cap.open(0);
	if(!cap.isOpened())
	{
	    std::cout << "could not open camera!" << std::endl;
	    exit(1);
	}
	
	// m_cap.set(CV_CAP_PROP_FRAME_WIDTH, m_width);
	// m_cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_height);
	// cout << "Camera successfully opened (ignore error messages above...)" << endl;
	// cout << "Actual resolution: "
	//      << m_cap.get(CV_CAP_PROP_FRAME_WIDTH) << "x"
	//      << m_cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    }

    void getRelPosition(AprileTags::TagDetection& detection, Eigen::Matrix3d& relRot, Eigen::Vector3d& relPos) {
	detection.getRelativeTranslationRotation(m_tagSize, m_fx, m_fy, m_px, m_py, relPos, relRot)
    }

    void getGlobalPosition(){
    }

    void processImage(cv::Mat& image, cv::Mat& image_gray) {
	// potential speed up offered by Michael Kaess
	// alternative way is to grab, then retrieve; allows for
	// multiple grab when processing below frame rate - v4l keeps a
	// number of frames buffered, which can lead to significant lag
	//      m_cap.grab();
	//      m_cap.retrieve(image);

	// Convert to gray scale image -- easier to just use grayscale camera?
	cv::cvtColor(image, image_gray, CVBGR2GRAY);

	// Detect tags
	if (m_timing) {
	    double t0 = tic();
	}
	vector<AprilTags::TagDetection> detections = m_tagDetector->extractTags(image_gray);
	if (m_timing) {
	    double dt = tic()-t0;
	    cout << "Extracting tags took " << dt << " seconds." << endl;
	}

	// Get relative positions to each tag
	Eigen::Matrix3d relRot[detections.size()];
	Eigen::Vector3d relPos[detections.size()];
	for (int i=0; i<detections.size(); i++) {
	    getRelPosition(detections[i], relRot, relPos);
	}

	// Get absolute position in real space
	
	double x, y, z, yaw, pitch, roll

	// Publish position to lcm
	
    }

    void loop() {
	cv::Mat image;
	cv::Mat image_gray;

	// for timing purposes
	double last_t = tic();
	while (true) {

	    // // capture frame
	    // m_cap >> image;
	    cap >> image;

	    // do actual image processing
	    processImage(image, image_gray);

	}
    }

// Start the main loop

int main( int argc, char* argv[])
{
    Detect detect;

    detect.parseOptions(argc, argv);
  
    // setup detector and video capture
    detect.setup();
  
    // processing loop for tag detection, orientation measurement, lcm generation
    detect.loop();

    return 0;
}
