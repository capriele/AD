/*
 * File:   locator.cpp
 * Author: Guivenca
 *
 * Created on July 17, 2015
 */

#include <cstdlib>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <stdio.h>

#include "../pgr2opencv.cpp"
//#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std; /*
 * File:   newmain.cpp
 * Author: Guivenca
 *
 * Created on June 22, 2015, 2:08 PM
 */

#include <cstdlib>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;

/*
 ****************************************************************************
 */
float dist(Point2f A, Point2f C)
{
    Point2f diff = A - C;
    return sqrt(diff.x * diff.x + diff.y * diff.y);
}

struct Polygon
{
    vector<Point> points;
    int area;

    Polygon();
    Polygon(int);
    Polygon(int, vector<Point>);
    int size();

    inline bool operator<(const Polygon& other) const;

};

Polygon::Polygon()
{
    area = 0;
}
Polygon::Polygon(int n)
{
    area = 0;
    points.resize(n);
    for(int k = 0; k < n; k++)
        points[k] = (Point(0, 0));
}
Polygon::Polygon(int a, vector<Point> vec)
{
    area = a;
    points = vec;
}
int Polygon::size()
{
    return points.size();
}

inline bool Polygon::operator<(const Polygon& other) const
{
    return this->area < other.area;
}



bool findReferenceRectangle(const vector<Polygon> poly3, const vector<Polygon> poly4, Polygon* rect_base, Polygon* triang_base)
{
    bool found = false;
    int min_distance, max_distance;
    int pointy_i, base_i;

    for(int i = 0; i < poly3.size(); i++)
        for(int j = 0; j < poly4.size(); j++)
        {
            //find the pointy corner of the triangle
            int side[3];
            pointy_i = 0;
            side[2] = dist(poly3[i].points[0], poly3[i].points[1]);
            side[0] = dist(poly3[i].points[1], poly3[i].points[2]);
            side[1] = dist(poly3[i].points[2], poly3[i].points[0]);
            if(side[0] > side[1]) pointy_i = 1;
            if(side[pointy_i] > side[2]) pointy_i = 2;


            //find the base point of the quadrilateral
            base_i = 0;
            min_distance = dist(poly4[j].points[0], poly3[i].points[pointy_i]);
            max_distance = min_distance;
            for(int k = 1; k < 4; k++)
            {
                int new_distance = dist(poly4[j].points[k], poly3[i].points[pointy_i]);
                ;
                if(min_distance > new_distance)
                {
                    min_distance = new_distance;
                    base_i = k;
                }
                if(max_distance < new_distance)max_distance = new_distance;

            }
            //TODO test criteria
            // TODO be careful with division by 0. It seems safe unless the area
            //is not enforced to be higher than something
            //cross ratio and alignemnt are less approximate criteria
            if(((double)min_distance / max_distance) < 0.3 && (((double)min_distance / max_distance) > 0.05) && (poly4[j].area > 3 * poly3[i].area))
            {

                //cout<<"ratio "<<(double)min_distance / max_distance<<endl;
                for(int k = 0; k < 3; k++)
                {
                    triang_base->points[k] = poly3[i].points[(k + pointy_i) % 3];
                }

                for(int k = 0; k < 4; k++)
                {
                    rect_base->points[k] = poly4[j].points[(k + base_i) % 4];
                }

                return true;
            }


        }



    return found;
};

int main(int argc, char** argv)
{

    Mat distCoeffs(5, 1, cv::DataType<double>::type);
    distCoeffs.at<double>(0) = -4.1802327018241026e-001;
    distCoeffs.at<double>(1) = 5.0715243805833121e-001;
    distCoeffs.at<double>(2) = 0.;
    distCoeffs.at<double>(3) = 0.;
    distCoeffs.at<double>(4) = -5.7843596847939704e-001;
    Mat rvec(3, 1, cv::DataType<double>::type);
    Mat tvec(3, 1, cv::DataType<double>::type);
    pgr2cv::VideoCapture cap;
    VideoWriter video ;
    string imagesPath;

    Mat src, src_gray, gray, drawing;
    int thresh_up = 170;//123;//90
    int thresh_low = 0;//0
    int frame_trackbar = 1;
    int frame = 0;
    int frame_max = 500;
    bool runVid = false;

    int eps = 7; //orig 7

    //Check if images from camera or from file (if file, provide folder as third call argument)
    bool isImgFromFile = false;
    bool isWriteInput  = false;
    bool isWriteResults = false;
    bool isDraw = false;

    //argument handling
    if(argc < 3)
    {
        cout << "Please provide flag if want to i) record input ii) record ouput iii) draw iv) path to simulated input (e.g. './locator 0 1 1 /home/ubuntu/')!" << endl;
        return -1;
    }

    else if(argc >= 4)
    {
        isWriteInput = atoi(argv[1]);
        isWriteResults = atoi(argv[2]);
        isDraw = atoi(argv[3]);
        cout << "write input: " << isWriteInput << " :: write ouput: " << isWriteResults << " :: draw: " << isDraw << endl;
    };

    if(argc >= 5)
    {
        isImgFromFile = true;
        frame_max = 504; 	//@TODO make flexible
        imagesPath = argv[4];
        cout << "Pull images from: " << imagesPath << endl;
    }
    else cout << "Camera needs to be plugged in!" << endl;


    //Camera handler
    if(!isImgFromFile)
    {
        cout << "Trying to connect to camera..." << endl;
        cap.open(0);
        cout << "Connection process ended." << endl;
        if(!cap.isOpened())
        {
            cout << "could not open camera!" << endl;
            return -1;
        }
        else cout << "camera open!" << endl;
    }

    double dWidth = 1280; //get the width of frames of the video
    double dHeight = 1024; //get the height of frames of the video

    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

    //init video if want to write to disk
    if(isWriteResults)
    {

        /*video.open("/home/ubuntu/Desktop/video2.avi",CV_FOURCC('M','J','P','G'), 20, frameSize, true); //initialize the VideoWriter object
        	//video.open("/home/ubuntu/Desktop/video2.avi",-1, 20, frameSize, true); //initialize the VideoWriter object
        if(!video.isOpened() ){
            	cout<<"could not open video!"<<endl;
            	return -1;
        		}
        */
    }

    //loop over either stream or file-images
    while(((!isDraw) && (waitKey(1) < 0)) || (isDraw))
    {
        //  for(int frame=1;frame<=frame_max;frame++){ //onyl 1 loop count if images come from camera

        pgr2cv::update_FPS(time(NULL));

        if((isDraw) && (waitKey(10) >= 0)) runVid = !runVid;

        //load file from image and convert to gray scale
        if(isImgFromFile)
        {

            if(runVid)
            {
                frame = frame + 1;
                if(frame > frame_max) frame = 1;
                frame_trackbar = frame;
            }
            else
                frame = frame_trackbar;


            ostringstream convert;
            //convert<<"C:\\Users\\ubuntu\\Desktop\\whiteboard test\\img-"<<frame<<".jpg";
            convert << imagesPath << "img-" << frame << ".jpg";
            string file = convert.str();
            src = imread(file);
            //cout<<"frame "<<frame<<endl;
            cvtColor(src, src_gray, CV_BGR2GRAY);
        }
        else
        {
            //read camera
            cap >> src_gray;
            cvtColor(src_gray, src, CV_GRAY2BGR);
            frame = frame + 1;
        }


        //threshold
        //imshow("gray",src_gray); waitKey(1000); show original now
        inRange(src_gray, thresh_low, thresh_up, gray);
        if(isDraw) imshow("gray", gray);

        //find the contours
        vector< vector<Point> > contours; // Vector for storing contour
        vector<Vec4i> hierarchy;
        findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE); //overrides input gray!

        vector<vector<Point> > contours_poly(contours.size());
        RNG rng(12345);

        int index = 13;
        //cout << " contours size: " << contours.size() << endl;


        // remove those too small or too complex
        // approximate those left.
        // make a separate list of quadrilateral/ triangle?

        vector<Polygon> poly3;
        vector<Polygon> poly4;
        poly3.resize(0);
        poly4.resize(0);
// /*
        for(int i = 0; i < contours.size(); i++)
        {
            int area = contourArea(contours[i], false);
            //if (contourArea(contours[i], false) > 30 && contours[i].size() < 2000) {
            if(area > 30 && contours[i].size() < 2000)
            {
                approxPolyDP(Mat(contours[i]), contours_poly[i], eps, true); //orig approx eps 7
                if(contours_poly[i].size() == 4 || contours_poly[i].size() == 3)
                {
                    Polygon new_p = Polygon(area, contours_poly[i]);
                    if(contours_poly[i].size() == 4) poly4.push_back(new_p);
                    if(contours_poly[i].size() == 3) poly3.push_back(new_p);
                }
            }
        }
        // */
        sort(poly3.rbegin(), poly3.rend());
        sort(poly4.rbegin(), poly4.rend());

        Polygon rect_base = Polygon(4);
        Polygon tri_base = Polygon(3);
        bool found = findReferenceRectangle(poly3, poly4, &rect_base, &tri_base);
        //cout << "N34poly: "<<poly3.size() <<" :: "<<poly4.size()<<" :: found reference rectangle? :" << found << endl;


        char pathinput[50]; sprintf(pathinput, "%s%d%s", "/home/ubuntu/Desktop/input_tmp/img-", frame, ".jpg");
        if(isWriteInput) imwrite(pathinput, src);//video<<src;


        if(isDraw) drawing = Mat::zeros(src.size(), CV_8UC3);

        //Draw reference rectangle and quadrilaterial onto bgr-source (src)
        if(found && isDraw)
        {

            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(src, vector<vector<Point> >(1, rect_base.points), -1, color, 1, 8);

            for(int k = 0; k < 4; k++)
            {
                color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                circle(src, rect_base.points[k], 3, color, -1);
            }



            color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            drawContours(src, vector<vector<Point> >(1, tri_base.points), -1, color, 1, 8);

            for(int k = 0; k < 3; k++)
            {
                color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                circle(src, tri_base.points[k], 3, color, -1);
            }

        }


        //Draw contours on new drawing


        if(isDraw)
        {
            for(int i = 0; i < contours.size(); i++)
            {
                Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

                drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
                if(contourArea(contours[i], false) > 30 && contours[i].size() < 2000)
                {

                    //TODO maybe have a more accurate aproximation once you know
                    //the targets
                    // this redoes the approx to polygons
                    approxPolyDP(Mat(contours[i]), contours_poly[i], 7, true);


                    if(contours_poly[i].size() == 4 || contours_poly[i].size() == 3)
                    {
                        drawContours(drawing, contours_poly, i, color, 2, 8, hierarchy, 0, Point());
                        color = Scalar(255, 255, 255);
                        for(int j = 0; j < contours_poly.at(i).size(); j++)
                        {
                            circle(drawing, contours_poly.at(i).at(j), 3, color, -1);
                            //imagePoints.at(j)=contours_poly.at(i).at(j);
                            //cout<<"Point "<<j<<" : "<<contours_poly.at(i).at(j)<<endl;
                            //diff.
                        }

                        //Pose reconstruction

                        //solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
                        //cout<< "index: "<<i<<endl;
                        //cout<<"tvec: "<<tvec<<endl;
                        //cout<<"rvec: "<<rvec<<endl;
                        //cout<<endl;
                    }
                }
            }

            /// Show in a window
            namedWindow("gray", WINDOW_AUTOSIZE);//CV_WINDOW_NORMAL
            createTrackbar("threshold upper", "gray", &thresh_up, 255);
            createTrackbar("threshold lower", "gray", &thresh_low, 255);

            namedWindow("view", WINDOW_AUTOSIZE);//CV_WINDOW_NORMAL
            createTrackbar("frame", "view", &frame_trackbar, 504);

            namedWindow("Contours", WINDOW_AUTOSIZE);//CV_WINDOW_NORMAL
            createTrackbar("epsilon", "Contours", &eps, 30);

            imshow("view", src);
            //imshow("gray",gray);	//shown earlier as findCountours overrides!
            //imshow("polygon",gray);
            imshow("Contours", drawing);
            //waitKey(25);


        }//endif isDraw

        //}//for-image loop(?)
        char pathoutput[50]; sprintf(pathoutput, "%s%d%s", "/home/ubuntu/Desktop/results_tmp/img-", frame, ".jpg");
        if(isWriteResults) imwrite(pathoutput, src);//video<<src;



    } //end while loop

    video.release();
    return 0;
}

