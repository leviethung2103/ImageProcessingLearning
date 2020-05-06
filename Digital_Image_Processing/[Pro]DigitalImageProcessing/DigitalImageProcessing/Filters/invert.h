#pragma once

#include "opencv2/opencv.hpp"
#include "iostream"
#include "stdio.h"

using namespace std;
using namespace cv;

void invertImage()
{
	Mat src, gray_image; 

	char* source_window = "Source Image";
	char* inverted_window = "Inverted Image";

	// Load the source image with 3-channel color image
	src = imread("../../SourceImage/lena.jpg", 1);

	//Conver it to the grayscale
	cvtColor(src, gray_image, CV_BGR2GRAY);


	int M = src.size().width; 
	int N = src.size().height;

	//Show the gray image
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, gray_image);

	// iterate over all image coordinates (u,v) | I(u,v) = 255 - I(u,v)
	
	for (int u = 0; u < M; u++)
	{
		for (int v = 0; v < N; v++)
		{
			gray_image.at<uchar>(v, u) = 255 - gray_image.at<uchar>(v,u);
		}
	}
	
	//Show the inverted image
	namedWindow(inverted_window, WINDOW_AUTOSIZE);
	imshow(inverted_window, gray_image);

	//Waits for a pressed key.
	// Display the window infinitely until any keypress
	waitKey(0);
}

