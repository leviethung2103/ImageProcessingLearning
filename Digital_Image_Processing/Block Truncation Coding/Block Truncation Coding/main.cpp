//Standard Library
#include "iostream"
//OpenCV library
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;


void main (void)
{
	// Load image
	Mat image = imread("lena.jpg",1);
	cvtColor(image, image, CV_BGR2GRAY);

	//Display the image
	namedWindow("Original Image");
	imshow("Original Image", image);


	for (int i = 0; i < image.rows; i = i+4) {
		for (int j = 0; j < image.cols; j = j+4) {
			double sum = 0.0;
			for (int i2= 0; i2 < 4; i2++) {
				for (int j2=0; j2 < 4; j2++) {
					sum += image.at<uchar>(i2 + i, j2 + i);
				}
			}
			double x_average = 0.0;
			x_average = sum / (image.rows*image.cols);
		}
	}

	waitKey(0);
}