// Ordered dithering using Bayer 5 Dither Array


#include "opencv2/opencv.hpp"
#include "iostream"
#include "stdio.h"
#define DEBUG 0


using namespace std;
using namespace cv;

void orderedDithering()
{
	Mat src, gray;

	char *source_window = "Source Image";
	char *ordered_window = "Ordered Image";

	//Load the source image with 3 channel color 
	src = imread("../../SourceImage/lena.jpg", 1);

	//Convert it t the grayscale
	cvtColor(src, gray, CV_BGR2GRAY);

	int cols = gray.cols;
	int rows = gray.rows;

	//Show the gray image
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, gray);

	//Declare matrix 8row x 8col
	float bayer[8][8] =
	{ { .513, .272, .724, .483, .543, .302, .694, .453 },
	{ .151, .755, .091, .966, .181, .758, .121, .936 },
	{ .634, .392, .574, .332, .664, .423, .604, .362 },
	{ .060, .875, .211, .815, .030, .906, .241, .845 },
	{ .543, .302, .694, .453, .513, .272, .724, .483 },
	{ .181, .758, .121, .936, .151, .755, .091, .966 },
	{ .664, .423, .604, .362, .634, .392, .574, .332 },
	{ .030, .906, .241, .845, .060, .875, .211, .815 } };

	// Get the bayer interger array
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bayer[i][j] = int(255 * bayer[i][j] + 0.5);
		}
	}

	// Notation: rImg: rowImage 
	// rDA: rowDitherArray
	for (int rImg = 0; rImg < rows / 8; rImg++)
	{
		for (int cImg = 0; cImg < cols / 8; cImg++)
		{
			for (int rDA = 0; rDA < 8; rDA++)
			{
				for (int cDA = 0; cDA < 8; cDA++)
				{
					if (gray.at<uchar>(rDA + 8 * rImg, cDA + 8 * cImg) > bayer[rDA][cDA])
					{
						gray.at<uchar>(rDA + 8 * rImg, cDA + 8 * cImg) = 255;
					}
					else
						gray.at<uchar>(rDA + 8 * rImg, cDA + 8 * cImg) = 0;
				}
			}

		}
	}

	//Show the gray image
	namedWindow(ordered_window, WINDOW_AUTOSIZE);
	imshow(ordered_window, gray);

	if (DEBUG == 1)
	{
		// Display the Bayer Matrix
		cout << "Bayer Matrix" << endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				cout << bayer[i][j] << '\t';
			}
			cout << endl;
		}

		cout << endl;
	}


	waitKey(0);
}