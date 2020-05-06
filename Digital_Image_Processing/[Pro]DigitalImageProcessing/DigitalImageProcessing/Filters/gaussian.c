#include "gaussian.h"

void gaussian() 
{
	// Using Gauss Class
	gauss g;

	// Show the gray scale image
	Mat image = g.loadImage();

	// Display Gray scale Image
	g.displayImage("Image",image);

	// Clone the image
	Mat copyImage = g.cloneImage(image);

	// Filter Average 3x3
	Mat result  = g.Filter_average3x3(image,copyImage);
	g.displayImage("Filter_Average3x3",result);

	// Filter Average 3x3
	result = g.Fitler_smoothing3x3(image,copyImage);
	g.displayImage("Filter_Smoothing3x3",result);

	// Fiter with gaussian kernel
	result = g.convGaussFilter(copyImage);
	g.displayImage("Filter_GaussianSmoothing",result);

	// Waitkey
	waitKey(0);

}