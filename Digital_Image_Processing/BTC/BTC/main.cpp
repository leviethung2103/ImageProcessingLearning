/*
* Block Truncation Coding
*/

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

int BTC(Mat &image, int block_size = 4);

void main(void)
{
	// Load Image
	Mat src = imread("../../SourceImage/lena.jpg", 1);
	//Conver it to the grayscale
	cvtColor(src, src, CV_BGR2GRAY);

	char* source_window = "Source Image";
	char* modified_window = "Modified Image";

	//Show the gray image
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, src);

	//Run Block Truncation Coding
	BTC(src);

	// Result
	namedWindow(modified_window, WINDOW_AUTOSIZE);
	imshow(modified_window, src);
	imwrite("BTC.jpg", src);

	waitKey(0);
}

int BTC(Mat &image, int block_size)
{
	// Calculate x_average
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j+4)
		{
			// Declare variables
			double x_average = 0.0; 
			double x_average2 = 0.0;
			double sigma =0.0;
			double sum1 = 0.0;
			double sum2 = 0.0;
			int q = 0;
			int a = 0;
			int b = 0;
			int m = 16;

			for (int i2 = 0; i2 < 4; i2++)
			{
				for (int j2 = 0; j2 < 4;  j2++)
				{
					sum1 += image.at<uchar>(i + i2, j + j2);
					sum2 += image.at<uchar>(i+i2,j+j2)*image.at<uchar>(i+i2,j+j2);
				}
			}

			x_average = sum1 / 16;
			x_average2 = sum2 / 16;
			sigma = sqrt(x_average2 - pow(x_average,2.0));

			for (int i3 = 0; i3 < 4; i3++)
			{
				for (int j3 = 0; j3 < 4; j3 ++)
				{
					if (image.at<uchar>(i3+i,j3+j) < x_average) 
						q = q +1;
				}
			}

			//Get mean a and b
			if (q == 0 || (m-q) == 0) {
				a = x_average;
				b = x_average;
			} else {
				a = int(x_average - sigma*sqrt(q/(m-q)));
				b = int(x_average + sigma*sqrt((m-q)/q));
			}

			// Get bitmap
			for (int i4 = 0; i4 < 4; i4++)
			{
				for (int j4 = 0; j4 < 4; j4 ++)
				{
					if (image.at<uchar>(i4+i,j4+j) < x_average) 
						image.at<uchar>(i4+i,j4+j) = a;
					else 
						image.at<uchar>(i4+i,j4+j) = b;
				}
			}
			
		}
	}
	return 1;
}

