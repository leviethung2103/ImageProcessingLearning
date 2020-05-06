//Standard Library
#include "stdafx.h"
#include "iostream"
#include "vector"
//OpenCV library
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;


class errDiff {
	private:
		Mat original = imread("../../SourceImage/lena.jpg", 0);

		int Thresholding = 128;
	public:
		Mat zeropadding(Mat image)
		{
					
			Mat zeroMatrix(image.rows+2,image.cols+2, CV_8UC1);
			for (int i = 0; i < image.rows+2; i++)
			{
				for (int j = 0; j < image.cols+2; j++)
				{
					zeroMatrix.at<uchar>(i,j)  = 0 ;
				}
			}
			return zeroMatrix;
			// Create an empty matrix with the larger size of original image
			
		}

		Mat depadding(Mat image)
		{

			// Create zero Matrix 
			Mat zeroMatrix(image.rows - 2, image.cols - 2, CV_8UC1);
			for (int i = 0; i < image.rows -2; i++)
			{
				for (int j = 0; j < image.cols - 2; j++)
				{
					zeroMatrix.at<uchar>(i, j) = 0;
				}
			}

			for (int i = 1; i <= image.rows-2; i++)
			{
				for (int j = 1; j <= image.cols-2; j++)
				{
					zeroMatrix.at<uchar>(i-1, j-1) = image.at<uchar>(i, j);
				}
			}
			return zeroMatrix;
		}

		Mat maskImage(Mat zeroMatrix, Mat image)
		{
			for (int i = 1; i < zeroMatrix.rows - 1; i++ )
			{
				for (int j = 1; j < zeroMatrix.cols - 1; j++)
				{
					zeroMatrix.at<uchar>(i,j) = image.at<uchar>(i-1,j-1);
				}
			}
			return zeroMatrix;
		}


		Mat diff(Mat image)
		{	

			for (int r =  1; r < image.rows-1; r++)
			{
				for (int c = 1; c < image.cols - 1; c ++)
				{
					int error = 0;

					if (image.at<uchar>(r,c) < Thresholding)
					{
						error = image.at<uchar>(r,c) - 0;
						image.at<uchar>(r, c) = 0;
					}
					else
					{
						error = image.at<uchar>(r,c) -255;
						image.at<uchar>(r,c) = 255;
					}

					image.at<uchar>(r,c+1) = smoothValue((7.0*error/16.0) + image.at<uchar>(r,c+1));
					image.at<uchar>(r+1,c-1) = smoothValue((3.0*error/16.0) + image.at<uchar>(r+1,c-1));
					image.at<uchar>(r+1,c) = smoothValue((5.0*error/16.0) + image.at<uchar>(r+1,c));
					image.at<uchar>(r+1,c+1) = smoothValue((1.0*error/16.0) + image.at<uchar>(r+1,c+1));
				}
			}
			//Depadding Image
			return depadding(image);

		}

		// Clamping Result
		int smoothValue(int value)
		{
			if (value > 255)
			{
				value = 255;
			}
			else if (value < 0)
			{
				value = 0;
			}

			return value;
		}

		Mat loadImage() 
		{
			if (preloadImage())
				return original;
		}

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
			cv::namedWindow(name);
			cv::imshow(name, image);
		}
};