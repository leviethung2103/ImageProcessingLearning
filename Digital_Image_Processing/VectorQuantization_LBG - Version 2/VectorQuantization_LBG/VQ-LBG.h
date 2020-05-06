/*
* File name:	VQ-LBG.h
*
* Created on:	Nov 24, 2017
* Author:		Le Viet Hung
*/

#ifndef VQ_LBG_H
#define VQ_LBG_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>	
#include <cstdlib>	/* srand, rand */
#include <ctime>	/* time */

#define BLOCK_SIZE 8
#define DIMENSION_CODE_VECTOR 64
#define LENGTH_CODE_BOOK 64
#define THRESHOLD 0.005

using namespace std;
using namespace cv;

int LBG(Mat &image);
int initTrainingSet(Mat image, Mat temp, Mat &trainingSet);
int initCodebook(Mat &codebook, Mat trainingSet, Mat temp, int LengthTrainingSet);
double distortion(Mat codebook, Mat indexClosetMatch, Mat trainingSet);
int updateCodebook(Mat container_cb, Mat &codebook, Mat count_container);
Mat createCompressedImage(Mat indexClosetMatch, Mat codebook, Mat image);
int findIndex(Mat trainingSet, Mat &codebook, int i);
#endif