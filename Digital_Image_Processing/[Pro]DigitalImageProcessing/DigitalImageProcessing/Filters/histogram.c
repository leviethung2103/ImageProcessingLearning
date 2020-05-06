// Histogram.cpp : Defines the entry point for the console application.
//

//Standard Library
#include "stdafx.h"
#include "iostream"
//OpenCV library
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "histogram.h"

using namespace std;
using namespace cv;

void histogram()
{
	Histogram hist;

	// Load image
	Mat image = hist.loadImage();

	//Display the image
	hist.displayImage("Orignal Image", image);

	// Generate the histogram
	int histogram[256];
	hist.imhist(image, histogram);

	// Compute the cumulative histogram
	int cumhistogram[256];
	hist.cumihst(histogram,cumhistogram);

	// Equalize Histogram
	hist.equalize(image,cumhistogram);

	//Display the result
	hist.displayImage("Result",image);

	//Display the histogram
	hist.histDisplay(histogram, "histogram");

	waitKey(0);
};

