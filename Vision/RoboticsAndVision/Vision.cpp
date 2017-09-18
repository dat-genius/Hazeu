// Vision.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

#define CircleMinSize 10
#define CircleMaxSize 25

using namespace cv;
using namespace std;

VideoCapture cap(2); //0 is the camera on the pc. Higher number is different camera. Insert filename to play video frame by frame


int main()
{
	//namedWindow("Camera Window", WINDOW_AUTOSIZE);
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	if (cap.isOpened()) {
		while (1) {
			Mat image, grayimage;

			cap.read(image);

			if (!image.empty()) {
				/// Convert it to gray
				medianBlur(image, image, 9);
				cvtColor(image, grayimage, CV_BGR2GRAY);

				/// Reduce the noise so we avoid false circle detection
				//GaussianBlur(grayimage, grayimage, Size(9, 9), 2, 2);

				vector<Vec3f> circles;

				/// Apply the Hough Transform to find the circles
				HoughCircles(grayimage, circles, CV_HOUGH_GRADIENT, 1, grayimage.rows / 16, 100, 30, CircleMinSize, CircleMaxSize);

				/// Draw the circles detected
				for (size_t i = 0; i < circles.size(); i++)
				{
					Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
					int radius = cvRound(circles[i][2]);
					// circle center
					circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
					// circle outline
					circle(image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
				}

				/// Show your results
				imshow("Hough Circle Transform Demo", image);
				circle(image, Point(15, 15), 6, Scalar(0, 0, 255), 1); //color is in BGR format NOT RGB!
				putText(image, "This is some text", Point(30, 30), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 255));

				cv::Vec3b pixel = image.at<cv::Vec3b>(300, 250); // read pixel (0,0) (make copy)
				cout << "HSV value is: " << (uint)pixel[0] << "," << (uint)pixel[1] << "," << (uint)pixel[2] << endl;

				//imshow("Camera Window", image);
				waitKey(10); //just a simple delay. Enter 0 for infinite wait till keypress
			} else {
				cout << "End of the file or camera disconnected!" << endl;
				system("Pause");
			}

		}

	} else {
		cout << "Could not open camera or file!" << endl;
		system("Pause");
	}

    return 0;
}

