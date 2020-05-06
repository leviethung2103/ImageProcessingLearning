/*
* The program is to add noises to images. 
* Gaussian Noise and Salt and Pepper Noise
*/


//Standard Library
#include "iostream"
#include "vector"
#include "math.h"
//OpenCV library
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void addGaussianNoise(Mat &image, double average = 0.0, double standard_deviation = 10.0);
void addSaltAddPepperNoise(Mat &image, double noise_percentage = 100);

void main()
{
	char *input_image = "lena.jpg";

	// Read Image 
	Mat image = imread(input_image, 1);
	//cvtColor(image, image, CV_BGR2GRAY);

	// Show the Image
	namedWindow("Image", CV_WINDOW_NORMAL);
	imshow("Image", image);

	// Add Gaussian Noise
	//addGaussianNoise(image);
	//namedWindow("Gaussian Noise", CV_WINDOW_NORMAL);
	//imshow("Gaussian Noise", image);
	// Add Salt and Pepper Noise
	addSaltAddPepperNoise(image);
	namedWindow("Salt&Pepper Noise", CV_WINDOW_NORMAL);
	imshow("Salt&Pepper Noise", image);

	waitKey(0);
	system("pause");
}

void addGaussianNoise(Mat &image, double average, double standard_deviation)
{
	Mat noise_image(image.size(), CV_16SC3);
	randn(noise_image, Scalar::all(average), Scalar::all(standard_deviation));

	Mat temp_image;
	image.convertTo(temp_image, CV_16SC3);
	addWeighted(temp_image, 1.0, noise_image, 1.0, 0.0, temp_image);
	temp_image.convertTo(image, image.type());
}

void addSaltAddPepperNoise(Mat &image, double noise_percentage)
{
	int image_rows = image.rows;
	int image_columns = image.cols;
	int image_channels = image.channels();
	int noise_points = (int)(((double)image_rows* image_columns*image_channels)*noise_percentage / 100.0);

	for (int count = 0; count < noise_points; count++)
	{
		int row = rand() % image_rows;
		int column = rand() % image_columns;
		int channel = rand() % image_channels;
		uchar* pixel = image.ptr<uchar>(row) + (column*image_channels) + channel;
		*pixel - (rand() % 2 == 1) ? 255 : 0; 

	}
}

