#pragma once

//Standard Library
#include "stdafx.h"
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


class gauss {
	private:
		Mat original = imread("../../SourceImage/lena.jpg", 0);
	public:
		Mat loadImage() 
		{
			if (preloadImage())
				return original;
		}


		// Check the image is valid before loading image
		int preloadImage()
		{
			if (!original.data)
			{
				cout << " nodata";
				return 0;
			}
			else
				return 1;
		}

		// Display image
		void displayImage(char *name, Mat image)
		{
			namedWindow(name);
			imshow(name, image);
		}

		// Clone the image

		Mat cloneImage(Mat image)
		{
			Mat output = image.clone();
			return output;
		}

		Mat Filter_average3x3(Mat image, Mat copy)
		{

			int rows = copy.rows;
			int cols = copy.cols;

			for (int r = 1; r <= rows - 2; r++)
			{
				for (int c = 1; c <= cols - 2; c ++)
				{
					// compute the filter result for postion (r,c)
					int sum = 0;
					for (int i = -1; i <= 1; i++)
					{
						for (int j = -1; j <= 1; j++)
						{
							int p = copy.at<uchar>(r+i,c+j);
							sum = sum + p;
						}
					}
					int  hotspot = (int) (sum/9.0);
					image.at<uchar>(r,c) = hotspot;

				}
			}

			return image;
		}

		Mat Fitler_smoothing3x3(Mat image, Mat copy)
		{
			// 3x3 filter matrix
			float H[3][3] = { { 0.075, 0.125, 0.075 },
							{ 0.125, 0.200, 0.125 },
							{ 0.075, 0.125, 0.075 } };

			int rows = copy.rows;
			int cols = copy.cols;


			// Move filter over the image coordinates
			for (int r = 1; r <= rows - 2; r++)
			{
				for (int c = 1; c <= cols - 2; c ++)
				{
					// compute the filter result for postion (r,c)
					// Iterate over the (i,j) coordinates within the rectangular filter region
					double sum = 0;
					for (int i = -1; i <= 1; i++)
					{
						for (int j = -1; j <= 1; j++)
						{
							int p = copy.at<uchar>(r+i,c+j);
							float c = H[j+1][i+1];
							sum = sum + p*c;	
						}
					}
					int  hotspot = (int) round(sum);
					image.at<uchar>(r,c) = hotspot;

				}
			}

			return image;
		}


		/*
		* Input : sigma , gaussian kernel size
		* Output : gaussian kernel 
		*/

		Mat makeGaussKernel2d(double sigma, int gKernelSize)
		{
			double s = 2*sigma*sigma; 

			// Make an empty Gaussian Kernel 
			Mat gKernel(gKernelSize,gKernelSize,CV_64F,Scalar::all(0));

			//Calculate the range for loop 2n+1 = size
			int range = gKernelSize/2;

			// Sum for normilazation = 1
			double sum = 0.0;

			// gernerate kernel by equation
			for (int x = -range; x <=range; x++)
			{
				for (int y = -range; y <=range; y++)
				{
					gKernel.at<double>(x+range,y+range) = (exp(-(x*x + y*y) / s)) / (3.14 * s);
					sum  += gKernel.at<double>(x+range,y+range);
				}
			}

			// normilize the kernel
			for (int i = 0 ; i < gKernelSize; i++)
			{
				for (int j = 0; j < gKernelSize; j ++)
				{
					gKernel.at<double>(i,j) = gKernel.at<double>(i,j)/sum;
				}
			}

			return gKernel;
		}

		/*
		* Input : Mat image 
		* Output : image
		*/

		Mat convGaussFilter(Mat image)
		{
			// Generate kernel with sigma = 1.5 and kernel size = 11x11
			Mat gKernel = gauss::makeGaussKernel2d(1.5, 11);


			// Get rows and cols of images
			int rows = image.rows;
			int cols = image.cols;

			//Calculate the range for loop by equation : 2n+1 = size
			int range = gKernel.rows/2;

			// Move filter over the image coordinates
			for (int r = range; r <= rows - range-1; r++)
			{
				for (int c = range; c <= cols - range-1; c ++)
				{
					// compute the filter result for postion (r,c)
					// Iterate over the (i,j) coordinates within the rectangular filter region
					double sum = 0;
					for (int i = -range; i <= range; i++)
					{
						for (int j = -range; j <= range; j++)
						{
							int p = image.at<uchar>(r+i,c+j);
							double c = gKernel.at<double>(i+range,j+range);
							sum = sum + p*c;	
						}
					}
					int  hotspot = (int) round(sum);
					image.at<uchar>(r,c) = hotspot;
				}
			}

			return image;
		}

		/**********************************************************************
		**************** Practical case for 5x5 with sigma = 1.0***************
		***********************************************************************/

		/*
		Mat makeGaussKernel2d()
		{
			double sigma = 1.0f;
			double s = 2.0*sigma*sigma;

			Mat gKernel(5,5,CV_64F,Scalar::all(0));

			// sum is for normailization
			double sum = 0.0;

			// generate 5x5 kernel
			for (int x = -2; x <=2; x++)
			{
				for (int y = -2; y <=2; y++)
				{
					gKernel.at<double>(x+2,y+2) = (exp(-(x*x + y*y) / s)) / (3.14 * s);
					sum  += gKernel.at<double>(x+2,y+2);
				}
			}

			// normalize the Kernel
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					gKernel.at<double>(i,j) /= sum;
				}
			}

			return gKernel;
		}
		
		Mat convGaussFilter(Mat image)
		{
			Mat gKernel = gauss::makeGaussKernel2d_new(1.5, 11);

			int rows = image.rows;
			int cols = image.cols;

			//Calculate the range for loop 2n+1 = size
			int range = gKernel.rows/2;

			// Move filter over the image coordinates
			for (int r = 2; r <= rows - 3; r++)
			{
				for (int c = 2; c <= cols - 3; c ++)
				{
					// compute the filter result for postion (r,c)
					// Iterate over the (i,j) coordinates within the rectangular filter region
					double sum = 0;
					for (int i = -2; i <= 2; i++)
					{
						for (int j = -2; j <= 2; j++)
						{
							int p = image.at<uchar>(r+i,c+j);
							double c = gKernel.at<double>(i+2,j+2);
							sum = sum + p*c;	
						}
					}
					int  hotspot = (int) round(sum);
					image.at<uchar>(r,c) = hotspot;
				}
			}

			return image;
		}

		*/

		


};