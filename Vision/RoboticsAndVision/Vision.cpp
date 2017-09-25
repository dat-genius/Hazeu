// Vision.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <regex>

#define CircleMinSize 10
#define CircleMaxSize 25

using namespace cv;
using namespace std;

VideoCapture cap(1); //0 is the camera on the pc. Higher number is different camera. Insert filename to play video frame by frame

Mat src; Mat src_gray; Mat src_hsv;
int circularity, thresh = 100;
int max_circularity, max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);
void circularity_callback(int, void*);

int main()
{
	//cvNamedWindow( "RGB", 1 );   
	cvNamedWindow("HSV", CV_WINDOW_KEEPRATIO);
	//cvNamedWindow( "Binary", 1 );  
	cvNamedWindow("LargestContour", CV_WINDOW_KEEPRATIO);
	cvNamedWindow("Final", CV_WINDOW_KEEPRATIO);
	Mat img, hsv, binary;
	while (1) {
		cap.read(img);
		//img = imread("C:\\Users\\Genius\\Pictures\\hazeu\\zonderflits2.jpg"); 
		//change this path according to your image file path  
		// imshow("RGB",img);  

		//convert RGB image into HSV image  
		cvtColor(img, hsv, CV_BGR2HSV);
		imshow("HSV", hsv);

		//get binary image  
		//inRange(hsv, Scalar(0, 255, 255), Scalar(179, 255, 255), binary);
		//get binary image  
		//inRange(hsv, Scalar(0, 85, 241), Scalar(18, 255, 255), binary);
		// imshow("Binary",binary);
		inRange(hsv, Scalar(0, 0, 200), Scalar(255, 255, 255), binary);
		// imshow("Binary",binary);  

		//find contours from binary image  
		int i;
		vector< vector<Point> > contours;
		findContours(binary, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE); //find contours  
		vector<double> areas(contours.size());
		//find largest contour area  
		for (i = 0; i < contours.size(); i++)
		{
			areas[i] = contourArea(Mat(contours[i]));
		}
		//get index of largest contour  
		double max;
		Point maxPosition;
		minMaxLoc(Mat(areas), 0, &max, 0, &maxPosition);
		//draw largest contour.  
		drawContours(binary, contours, maxPosition.y, Scalar(255), CV_FILLED);
		imshow("LargestContour", binary);

		//draw bounding rectangle around largest contour  
		Point center;
		Rect r;
		if (contours.size() >= 1)
		{
			r = boundingRect(contours[maxPosition.y]);
			rectangle(img, r.tl(), r.br(), CV_RGB(0, 0, 255), 3, 8, 0); //draw rectangle  
		}
		//get centroid  
		center.x = r.x + (r.width / 2);
		center.y = r.y + (r.height / 2);
		cout << center.x << " , " << center.y << endl;

		//print x and y co-ordinates on image  
		char x[15], y[6];
		/*	itoa(center.x, x, 10);
			itoa(center.y, y, 10);
			strcat(x, ",");
			putText(img, strcat(x, y), center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 0, 0), 1, CV_AA);*/
		imshow("Final", img);
		cvWaitKey();
	}

	//wait for key press  
	cvWaitKey();
	return 0;
}


/** @function main */
/*
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	src = imread("C:\\Users\\Genius\\Pictures\\hazeu\\zonderflits1.jpg");
	cvtColor(src, src_hsv, CV_BGR2HSV);
	namedWindow("HSV-Window", CV_WINDOW_KEEPRATIO);
	imshow("HSV-Window", src_hsv);

	/// Convert image to gray and blur it
	cvtColor(src_hsv, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_KEEPRATIO);
	imshow(source_window, src);

	createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, nullptr);

	waitKey(0);
	return(0);
}



/** @function thresh_callback */
/*
void thresh_callback(int, void*)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	/// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}


	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_KEEPRATIO);
	imshow("Contours", drawing);
}
/*
int main()
{
	// Read image
	Mat im;
	cout << "something something image" << endl;
	im = imread("C:\\Users\\Genius\\Pictures\\hazeu\\blob.jpg");     //photo_2017-09-20_16-02-23.jpg");
	cout << "something something image imported" << endl;

	SimpleBlobDetector::Params params;

	// Change the detectioncolor
	params.filterByColor = true;
	params.blobColor = 0x000000;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 150;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1f;
	params.maxCircularity = 0.81f;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.087f;
	params.maxConvexity = 1.0f;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.001f;
	params.maxInertiaRatio = 1.0f;

	// Set up the detector with default parameters.
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	cout << "something something trying to detect" << endl;
	detector->detect(im, keypoints);
	cout << "something something detected shit" << endl;

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	Mat im_with_keypoints;
	drawKeypoints(im, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	imshow("keypoints", im_with_keypoints);
	waitKey(0);

	/*
	//namedWindow("Camera Window", WINDOW_AUTOSIZE);
	namedWindow("Camera", CV_WINDOW_AUTOSIZE);
	if (cap.isOpened()) {
		while (1) {
			
		}

	} else {
		cout << "Could not open camera or file!" << endl;
		system("Pause");
	}

    return 0;
}
*/