/*
* File:				main.cpp
* Homework 2:		Vector Quantization - LBG Algorithm
* Course:			Multimedia Signal Processing
* Created on:		Nov 24, 2017
* Author:			Le Viet Hung
*/

#include "VQ-LBG.h"

void main(void)
{	
	// Load Image
	Mat src = imread("lena.bmp", 1);
	//Conver it to the grayscale
	cvtColor(src, src, CV_BGR2GRAY);
	//Show the gray image
	namedWindow("Source Image", WINDOW_AUTOSIZE);
	imshow("Source Image", src);

	//Run Vector Quantization
	LBG(src);

	waitKey(0);
}


