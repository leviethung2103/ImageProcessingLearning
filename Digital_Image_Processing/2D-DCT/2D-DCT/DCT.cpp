/*
* File name:	DCT.cpp
*
* Created on:	Nov 28, 2017
* Author:		Le Viet Hung
*/

#include "DCT.h"

/**
* @brief  DCT - 2D
* @param  Mat image, Mat &DCT_Image_1D, int posX, int posY
* @retval 1 
*/
int DCT(Mat image, Mat &DCT_Image, int posX, int posY)
{	
	double result = 0.0;
	// Scan one block 8x8
	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = 0; j  < BLOCK_SIZE; j++) {
			
			double Ci, Cj, z = 0.0;

			Ci = COEFFS(i);
			Cj = COEFFS(j);
			
			for (int x = 0; x < BLOCK_SIZE; x ++) {
				for (int y = 0; y < BLOCK_SIZE; y++) {
					double s, q;

					s = image.at<uchar>(x+posX,y+posY);

					q = s * cos(((double)(2*x+1) * (double)i * PI)/(2*BLOCK_SIZE))*
						    cos(((double)(2*y+1) * (double)j * PI)/(2*BLOCK_SIZE));
					z += q;
				}
			}
			result = round(0.25*Ci*Cj*z);
			DCT_Image.at<double>(i+posX,j+posY) = result;
		}
	}
	return 1;
}

/**
* @brief  inverse DCT - 2D
* @param  Mat image, Mat &DCT_Image_1D, int posX, int posY
* @retval 1 
*/
int iDCT(Mat DCT_Image, Mat &iDCT_Image, int posX, int posY)
{
	double result = 0.0;

	// Scan one block 8x8
	for (int x = 0; x < BLOCK_SIZE; x++) {
		for (int y = 0; y  < BLOCK_SIZE; y++) {
			
			double z = 0.0;

			for (int i = 0; i < BLOCK_SIZE; i ++) {
				for (int j = 0; j < BLOCK_SIZE; j++) {
					
					double s, q;
					double Ci, Cj;
					
					Ci = COEFFS(i);
					Cj = COEFFS(j);

					s = DCT_Image.at<double>(i+posX,j+posY);

					q = Ci*Cj* s * cos((double)(2*x+1) * (double)i * PI/(2*BLOCK_SIZE))*
						    cos((double)(2*y+1) * (double)j * PI/(2*BLOCK_SIZE));
					z += q;
				}
			}

			result = round(z*0.25);

			smoothValue(result);

			iDCT_Image.at<uchar>(x+posX,y+posY) = result;
		}
	}

	return 1;
}


/**
* @brief  Identify the coefficent
* @param  value
* @retval 1 or sqrt(1/2)
*/
double COEFFS(int value)
{
	double rst = 0.0;
	if (value == 0)
		return rst = 1.0 / sqrt(2.0);
	else
		return rst = 1.0;
}
/**
* @brief  Print the value
* @param  value
* @retval none
*/
void DEBUG(int value)
{
	cout << value << endl;
}

/**
* @brief  Clamping the result
* @param  value
* @retval 1
*/
int smoothValue(double &value)
{
	if (value > 255)
		value = 255;
	else if (value < 0)
		value = 0;
	return 1;
}