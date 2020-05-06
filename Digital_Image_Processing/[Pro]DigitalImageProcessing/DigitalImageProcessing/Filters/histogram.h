#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Histogram {
	private:
		
	public:
		void imhist(Mat image, int histogram[])
		{
			// initialize all intensity values to 0
			for (int i = 0; i < 256; i ++)
			{
				histogram[i] = 0;
			}

			// calculate the no of pixels for each intensity values
			for (int r = 0; r < image.rows; r++) 
			{
				for (int c=0; c < image.cols; c++) 
				{
					int i = (int)image.at<uchar>(r, c);
					histogram[i] = histogram[i] +1;
				}
			}
		}

		void cumihst(int histogram[], int cumhistogram[])
		{
			cumhistogram[0] = histogram[0];
			for (int i = 1; i < 256; i++)
			{
				cumhistogram[i] = cumhistogram[i-1] + histogram[i];
			}
		}


		// equalize the image
		void equalize(Mat &image, int histogram[])
		{
			int K = 256; // number of intensity values

			for (int r = 0; r < image.rows; r++) 
			{
				for (int c=0; c < image.cols; c++) 
				{ 
					int a = (int)image.at<uchar>(r,c);
					int d = histogram[a];
					int b = (histogram[a] * (K-1)) /(image.rows*image.cols); 
					image.at<uchar>(r,c) = b; 
				}
			}
		}

		// Display the histogram graph
		void histDisplay(int histogram[], const char* name)
		{
			int hist[256];
			for (int i = 0 ; i < 256; i++)
			{
				hist[i] = histogram[i];
			}

			// draw the histograms
			int hist_w = 512; int hist_h = 400;
			int bin_w = cvRound((double) hist_w/256);

			Mat histImage(hist_h,hist_w,CV_8UC1, Scalar(255,255,255));

			// Find the maximum intensity element from histogram
			int max = hist[0];
			for (int i = 1; i < 256; i++)
			{
				if (max < hist[i])
				{
					max = hist[i];
				}
			}

			// Normalize the histogram between 0 and histImage.rows
			for (int i = 0; i < 256; i ++)
			{
				hist[i] = ((double)hist[i]/max)*histImage.rows;
			}

			// draw the intensity line for histogram
		    for(int i = 0; i < 256; i++)
		    {
		        line(histImage, Point(bin_w*(i), hist_h),
		                              Point(bin_w*(i), hist_h - hist[i]),
		             Scalar(0,0,0), 1, 8, 0);
		    }

		    // display histogram
	       namedWindow(name, CV_WINDOW_AUTOSIZE);
	       imshow(name, histImage);

		}

		// Read input image
		Mat loadImage()
		{
			Mat image = cv::imread("../../SourceImage/lena.jpg", 0);
			if (!image.data)
			{
				cout << "There is no input data" << endl;
			}
			else 
				return image;
		}

		// Display image
		void displayImage(char *name, Mat image)
		{
			cv::namedWindow(name);
			cv::imshow(name, image);
		}

		
};