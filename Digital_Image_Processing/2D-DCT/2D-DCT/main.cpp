/*
* File:				main.cpp
* Homework 3:		2D-DCT
* Course:			Multimedia Signal Processing
* Created on:		Nov 28, 2017
* Author:			Le Viet Hung
*/

#include "DCT.h"

void main(void)
{
	// Load Image
	Mat src = imread("lena.bmp", 1);
	//Conver it to the grayscale
	cvtColor(src, src, CV_BGR2GRAY);
	//Show the gray image
	namedWindow("Source Image", WINDOW_AUTOSIZE);
	imshow("Source Image", src);


	/* 
	 * Run 2D-DCT  
	 */
	Mat DCT_Image(src.size(),CV_64F,Scalar::all(0));
	Mat iDCT_Image(src.size(), CV_8UC1,Scalar::all(0));

	int countBlock = 0;
		
	for (int i = 0; i < src.rows/BLOCK_SIZE; i++) {
		for (int j = 0; j < src.cols/BLOCK_SIZE; j++) {
				/* 2-D DCT */
				DCT(src,DCT_Image,i*8,j*8);
				/* Inverse 2-D DCT */
				iDCT(DCT_Image, iDCT_Image, i * 8, j* 8);
				cout << "Processed " << countBlock++ << " blocks" << endl;
		}
	}

	//Show Image after DCT
	namedWindow("DCT Image", WINDOW_AUTOSIZE);
	imshow("DCT Image", DCT_Image);
	imwrite("DCT_Image.jpg",DCT_Image);

	//Show Image after IDCT
	namedWindow("IDCT Image", WINDOW_AUTOSIZE);
	imshow("IDCT Image", iDCT_Image);
	imwrite("IDCT_Image.jpg",iDCT_Image);

	waitKey(0);
}


