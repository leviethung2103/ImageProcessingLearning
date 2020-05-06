#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class Histogram {
	private:
		int maxPixel = 0;
		int minPixel = 255;

		int maxIntensity = 255;
		int minIntensity = 0;

		int histArray[256] = { 0 };			// initialize all intensity values to 0

		int cols = 0;
		int rows = 0;

		int size = 0;

	public:
		
		// computeHistogram will return a pointer with int data type
		float getHistogram(const cv::Mat &image)
		{
			cv::Mat hist = image;

			 // Generate the histogram
			Histogram::computeHistogram(image);

			// Calculate the scaling factor
			Histogram::calculateAlpha(image);

			return Histogram::calculateAlpha(image);

			//return Histogram::acAdjustment(image);

		}

		int *computeHistogram(const cv::Mat &image)
		{		
			
			//Get the width and heigh of image
			cols = image.cols;
			rows = image.rows;

			// calculate the no of pixels for each intensity values
			for (int r = 0; r < rows; r++) 
			{
				for (int c=0; c < cols; c++) 
				{
					int i = (int)image.at<uchar>(r, c);
					histArray[i] = histArray[i] +1;
				}
			}


			
			return histArray;	
		}

		void showHistogramValues(int *histArray)
		{
			for (int i = 0; i < 256; i++)
			{
				cout << "Value " << i << " = " << histArray[i] << endl;

			}

		}

		float calculateAlpha (const cv::Mat &image)
		{
			//Get the width and heigh of image
			cols = image.cols;
			rows = image.rows;
			// Caluculate the scaling factor
			size = cols*rows;
			float alpha = 255.0/size;

			return alpha;
		}


		int findHighestPixel(const cv::Mat &image)
		{


			for (int r = 0; r < rows; r++) 
			{
				for (int c=0; c < cols; c++) 
				{
					if (image.at<uchar>(r, c) > maxPixel)
					{
						maxPixel = image.at<uchar>(r,c);
					}
				}
			}

			return maxPixel;
		}

		int findLowestPixel(const cv::Mat &image)
		{

			for (int r = 0; r < rows; r++) 
			{
				for (int c=0; c < cols; c++) 
				{
					if (image.at<uchar>(r, c) < minPixel)
					{
						minPixel = image.at<uchar>(r,c);
					}
				}
			}

			return minPixel;
		}

		// Automatic Contrast Adjustment
		cv::Mat acAdjustment(const cv::Mat &image)
		{
			cv::Mat dst = image;

			Histogram::findHighestPixel(dst);

			Histogram::findLowestPixel(dst);

			cout << "Max Pixel: " << maxPixel << "   " << minPixel << endl;

			for (int r = 0; r < rows; r++) 
			{
				for (int c=0; c < cols; c++) 
				{
					dst.at<uchar>(r,c) = ((dst.at<uchar>(r,c) - minPixel) * 255 ) / (maxPixel-minPixel);
				}
			}

			return dst;
		}

		// compute the cuculative histogram
		void cumhist(int histogram[], int cumhistogram[])
		{
			cumhistogram[0] = histogram[0];

			for (int i = 1; i <256; i++)
			{
				cumhistogram[i] = histogram[i] + cumhistogram[i-1];
			}
 		}
};