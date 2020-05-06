/*
* File name:	2D_DCT.h
*
* Created on:	Nov 28, 2017
* Author:		Le Viet Hung
*/

#ifndef DCT_H
#define DCT_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>	
#include <cstdlib>	/* srand, rand */
#include <ctime>	/* time */

/* Parameters*/
#define PI 3.14159265358979
#define BLOCK_SIZE 8

using namespace std;
using namespace cv;

/*Functions */
int DCT(Mat image, Mat &DCT_Image, int posX, int posY);
int iDCT(Mat DCT_Image, Mat &iDCT_Image, int posX, int posY);

double COEFFS(int value);
void DEBUG(int value);
int smoothValue(double &value);

#endif
