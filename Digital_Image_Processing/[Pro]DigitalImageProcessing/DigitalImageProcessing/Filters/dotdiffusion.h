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

class dot {
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

		Mat depadding(Mat image)
		{
			// Create zero Matrix 
			Mat zeroMatrix(image.rows - 2, image.cols - 2, CV_64F, Scalar::all(0));

			for (int i = 1; i <= image.rows-2; i++)
			{
				for (int j = 1; j <= image.cols-2; j++)
				{
					zeroMatrix.at<double>(i-1, j-1) = image.at<double>(i, j);
				}
			}
			return zeroMatrix;
		}


		// Extend the image size with padding 0 into the corners
		void padImage(Mat &image)
		{
			// Create an empty matrix with the larger size of original image
			// Image has size 512x512 => zeroMatrx = 514x514
			Mat zeroMatrix(image.rows+2,image.cols+2, CV_64F, Scalar::all(0));

			// Copy data from image to new matrix
			for (int i = 1; i < zeroMatrix.rows - 1; i++ )
			{
				for (int j = 1; j < zeroMatrix.cols - 1; j++)
				{
					zeroMatrix.at<double>(i,j) = image.at<double>(i-1,j-1);
				}
			}

			image = zeroMatrix;
		}

		/**
		  * @brief Extend the Class Matrix
		  * @param 
		  * @retval void
		  */
		Mat createClassMatrix (Mat image)
		{
			// Create Class Matrix;
			 double ClassMatrix[8][8] = {
				{34,48,	40,	32,	29,	15,	23,	31},
				{42,58,	56,	53,	21,	5,	7,	10},
				{50,62,	61,	45,	13,	1,	2,	18},
				{38,46,	54,	37,	25,	17,	9,	26},
				{28,14,	22,	30,	35,	49,	41,	33},
				{20,4,	6,	11,	43,	59,	57,	52},
				{12,0,	3,	19,	51,	63,	60,	44},
				{24,16,	8,	27,	39,	47,	55,	36}	
			};

			// Copy the array to Mat . I want to process all Mat type
			Mat CM_Knuth(8,8,CV_64F,ClassMatrix);		// pay attention to the data type CV_64F and CV_8UC1

			// Create the empty zero matrix 512x512
			Mat zeroMatrix(image.rows,image.cols, CV_64F, Scalar::all(0));

			// Extend / Replicate zeromatrix = size of image 512x512
			for (int r = 0; r < image.rows ; r = r +8 )
			{
				for (int c = 0; c < image.cols; c = c+8)
				{
					for (int i = 0; i < 8; i++)
					{
						for (int j=0; j<8; j++)
						{
							zeroMatrix.at<double>(r+i,c+j) = CM_Knuth.at<double>(i,j);
						}
					}
				}
			}

			// Create the empty zero matrix 514x514
			Mat zeroMatrix2(image.rows+2,image.cols+2, CV_64F, Scalar::all(-1));

			// Copy zeroMatrix to zeroMatrix2
			// Copy data from image to new matrix
			for (int i = 1; i < zeroMatrix2.rows - 1; i++ )
			{
				for (int j = 1; j < zeroMatrix2.cols - 1; j++)
				{
					zeroMatrix2.at<double>(i,j) = zeroMatrix.at<double>(i-1,j-1);
				}
			}

			return zeroMatrix2;
		}


		Mat convert2Double(Mat &src)
		{
			src.convertTo(src,CV_64F);

			// Copy data 
			for (int i =0; i < src.rows; i++) {
				for (int j=0; j< src.cols; j++) {
					src.at<double>(i,j) = src.at<double>(i,j) / 255.0;
				}
			}

			return src;
		}


		//	Dot diffusion proposed by Knuth
		Mat runDotDiffusion(Mat &image)
		{
			Mat CM = createClassMatrix(image);

			// Zeropadding for image
			padImage(image);

			// Create diffusion weight in Dot Diffusion
			double DiffusionWeight[3][3] = {
				{1,2,1},
				{2,0,2},
				{1,2,1}};

			Mat DW(3,3,CV_64F,DiffusionWeight);

			// Find the size of vector
			int VectorSize = 0;
			findSizeVector(CM,VectorSize);

			// Initialize rowVector and colVector to store the index value
			Mat rowVector = Mat::zeros(VectorSize, 64, CV_64F);
			Mat colVector = Mat::zeros(VectorSize, 64, CV_64F);

			for (int order = 0; order < 64; order ++ )
			{
				// Get the rowVector and colVector from CM Matrix 514x514
				// To store the index of the row and col
				getVector_new(order,CM,rowVector,colVector);
			}


			for (int order=0; order < 64; order ++)
			{
				// Get the rowVector and colVector from CM 514x514 matrix
				// To store the index of row and col
				//getVector(order,CM,rowVector,colVector);

				for (int count = 0; count < VectorSize; count++)
				{
					int row = rowVector.at<double>(count, order);
					int col = colVector.at<double>(count, order);


					double qerror = 0;

					if (image.at<double>(row, col) > 0.5)
					{
						qerror = image.at<double>(row, col) - 1;
						image.at<double>(row, col) = 1;
					}
					else
					{
						qerror = image.at<double>(row, col) - 0;
						image.at<double>(row, col) = 0;
					}

					// Get the 3x3 region values for computation
					Mat DW_new(3, 3, CV_64F, Scalar::all(0));

					for (int m = -1; m < 2; m++)
					{
						for (int n = -1; n < 2; n++)
						{
							DW_new.at<double>(m + 1, n + 1) = CM.at<double>(row + m, col + n);
						}
					}

					// Define the position to dispere
					findPositionToDisperse(DW_new, order);

					// After we defined which value is 0 or 1.
					// Multiple with the original DW 
					Mat DW_new2;
					multiply(DW_new, DW, DW_new2);

					double sum = 0.0;
					sum = sumAllWeight(DW_new2);


					// Consider the value 62 & 63
					if (sum != 0)
					{
						// Change the value surrounding
						for (int m2 = -1; m2 < 2; m2++)
						{
							for (int n2 = -1; n2 < 2; n2++)
							{
								image.at<double>(row + m2, col + n2)
									= clamping(image.at<double>(row + m2, col + n2) +
										qerror*DW_new2.at<double>(m2 + 1, n2 + 1) / sum);
							}
						}
					}
					
				}
			}
				
			return depadding(image);
			
		}

		/**
		  * @brief  Find the postion of value 0->63 in Class Matrix
		  * @param 	value need to be found, CM, rowVector, colVector
		  * @retval void
		  */
		void getVector (int value, Mat CM, Mat &rowVector, Mat &colVector)
		{
			int countIndex=0;
			for (int i = 0; i < CM.rows; i++)
			{
				for (int j = 0; j < CM.cols; j++)
				{
					if (CM.at<double>(i, j) == value)
					{
						rowVector.at<double>(countIndex, 0) = i;
						colVector.at<double>(countIndex, 0) = j;
						countIndex++;
					}
				}
			}
		}

		/**
		  * @brief  Find the postion of value 0->63 in Class Matrix
		  * @param 	value need to be found, CM, rowVector, colVector
		  * @retval void
		  */
		void getVector_new (int value, Mat CM, Mat &rowVector, Mat &colVector)
		{
			int countIndex=0;
			for (int i = 0; i < CM.rows; i++)
			{
				for (int j = 0; j < CM.cols; j++)
				{
					if (CM.at<double>(i, j) == value)
					{
						rowVector.at<double>(countIndex, value) = i;
						colVector.at<double>(countIndex, value) = j;
						countIndex++;
					}
				}
			}
		}


		/**
		  * @brief  Find the size of Vector 
		  * @param 	Class Matrix
		  * @retval size
		  */
		 void findSizeVector(Mat ClassMatrix,int &Size) 
		 {

			for (int i = 0; i < ClassMatrix.rows; i++)
			{
				for (int j = 0; j < ClassMatrix.cols; j++)
				{
					if (ClassMatrix.at<double>(i, j) == 0)
					{
						Size += 1;
					}
				}
			}
		 }

		 /**
		   * @brief  Sum all Weight
		   * @param  Diffused Matrix	
		   * @retval sum
		   */
		  double sumAllWeight(Mat DiffusedMatrix) 
		  {
		  	double sum = 0; 
		 	for (int i = 0; i < 3; i++)
		 	{
		 		for (int j = 0; j < 3; j++)
		 		{
		 			sum += DiffusedMatrix.at<double>(i, j);
		 		}
		 	}
		 	return sum;
		  }

		  /**
		    * @brief  Find Position to Disperse
		    * @param  Diffused Matrix, the value need to be compare	
		    * @retval none
		    */
		   void findPositionToDisperse(Mat &DM, double centerValue) 
		   {
		  		for (int i = 0 ; i < 3 ; i++) 
		  		{
		  			for (int j = 0 ; j < 3; j++)
		  			{
		  				if (DM.at<double>(i,j) > centerValue)
		  				{
		  					DM.at<double>(i,j) = 1;
		  				}
		  				else 
		  					DM.at<double>(i,j) = 0;
		  			}
		  		}
		   }

		   // Clamping Result
			double clamping(double value)
			{
				if (value > 1.0)
				{
					value = 1;
				}
				else if (value < 0)
				{
					value = 0;
				}

				return value;
			}



};