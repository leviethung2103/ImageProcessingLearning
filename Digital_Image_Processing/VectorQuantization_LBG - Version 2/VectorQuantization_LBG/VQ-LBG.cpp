/*
* File name:	VQ-LBG.cpp
*
* Created on:	Nov 24, 2017
* Author:		Le Viet Hung
*/

#include "VQ-LBG.h"

// LBG Algorithm
int LBG(Mat &image)
{	
	srand((unsigned)time(0));
	
	// Calculate the total pixels of image
	int NoPixelsImage = image.rows*image.cols;

	//  n = length of training set = total blockds in one image
	int LengthTrainingSet = NoPixelsImage / DIMENSION_CODE_VECTOR;

	// Declare training set = 4096x64
	Mat trainingSet(LengthTrainingSet, DIMENSION_CODE_VECTOR, CV_64F, Scalar::all(0));

	// Create a codebook with size 64x64
	Mat codebook(LENGTH_CODE_BOOK, DIMENSION_CODE_VECTOR, CV_64F, Scalar::all(0));

	// Declare temporary set 1x64 
	Mat temp(1, DIMENSION_CODE_VECTOR, CV_64F, Scalar::all(0));

	// Initilize Training Set
	initTrainingSet(image, temp, trainingSet);

	// Initilize Codebook 64x64
	initCodebook(codebook, trainingSet, temp,LengthTrainingSet);

	/******************* End Decleration *********************************/



	double x = 1.0;

	double distort_fact_prev = 1.0;
	double distort_fact = 1.0;

	int iterations = 0;

	// This stores the indices of the codevector present in codebook with a particular training vector
	Mat indexClosetMatch(1, LengthTrainingSet, CV_64F, Scalar::all(0));

	while (x > THRESHOLD)
	{
		iterations = iterations + 1;
		distort_fact_prev = distort_fact;

		// Container_cb same size as codebook
		// Create Nc classes
		Mat container_cb(LENGTH_CODE_BOOK, DIMENSION_CODE_VECTOR, CV_64F, Scalar::all(0));

		// Maintain the upper filled limit of the container_class
		Mat count_container(1, LENGTH_CODE_BOOK, CV_64F, Scalar::all(1));

		for (int i = 0; i < LengthTrainingSet; i++) {
			int index = findIndex(trainingSet, codebook, i);
			// Save the index after searching to indexCloseMatch
			indexClosetMatch.at<double>(0, i) = index;
			
			// Store the 64 elements of 1st, 2nd, 3rd block to container_cb with specified k
			for (int i2 = 0; i2 < DIMENSION_CODE_VECTOR; i2++) {
				container_cb.at<double>(index, i2) = container_cb.at<double>(index, i2) + trainingSet.at<double>(i, i2);
			}

			count_container.at<double>(0, index) = count_container.at<double>(0, index) + 1;
		}
		//This updates the codebook using the container_cb and count_containerupd
		updateCodebook(container_cb, codebook, count_container);

		// This function updates the value of distortion factor
		distort_fact = distortion(codebook, indexClosetMatch, trainingSet);

		// This calculates the converence of distortion factor
		x = abs(distort_fact_prev - distort_fact);
		x = x / distort_fact_prev;
		cout << x << endl;
	}


	// Create the compressed image using codebook and indexClosetMatch
	Mat result = createCompressedImage(indexClosetMatch, codebook, image);
	imwrite("Lena_4x4.jpg",result);
	// Result
	namedWindow("Modified Image", WINDOW_AUTOSIZE);
	imshow("Modified Image", result);

	return 1;
}



/**
* @brief  Initialize Codebook by randomly picking codevector from trainingSet
* @brief  Select 64 codevectors at random to be the inital codebook
* @param
* @retval void
*/
int initCodebook(Mat &codebook, Mat trainingSet, Mat temp, int LengthTrainingSet)
{
	for (int i = 0; i < LENGTH_CODE_BOOK; i++) {
		int random_number = rand() % LengthTrainingSet;
		for (int j = 0; j < DIMENSION_CODE_VECTOR; j++) {
			codebook.at<double>(i, j) = trainingSet.at<double>(random_number, j);
		}
	}
	return 1;
}

/**
* @brief  Initialize Training Set based on image and Number of vectors
* @param  image,temp,traingSet
* @retval void
*/
int initTrainingSet(Mat image, Mat temp, Mat &trainingSet)
{
	int indexRow = 0;
	int indexCol = 0;

	// First for loop for scan the image by block size 8x8
	for (int i1 = 0; i1 < image.rows; i1 = i1 + BLOCK_SIZE) {
		for (int j1 = 0; j1 < image.cols; j1 = j1 + BLOCK_SIZE) {
			// Second for scan the all elements inside a block = Get One Codevector
			for (int i2 = 0; i2 < BLOCK_SIZE; i2++) {
				for (int j2 = 0; j2 < BLOCK_SIZE; j2++) {
					// Third for save the every pixel inside a block to trainingSet
					trainingSet.at<double>(indexRow,indexCol) = image.at<uchar>(i2+i1,j2+j1);
					indexCol ++;
				}
			}
			// Reset indexCol to start a new mission
			indexCol = 0; 
			indexRow ++;
		}
	}
	return 1;
}

/**
* @brief  Find the minimum distortion in comparsion between codebook and training set
* @brief  Find the nearest neighbor clustering.
* @brief  Compare the first row of Trainingset with every row of Codebook
* @param
* @retval index of codebook
*/
int findIndex(Mat trainingSet, Mat &codebook, int i)
{
	int indexCol = codebook.cols;
	int indexRow = codebook.rows;

	int indexK = 0;

	// Set the max value for distance
	double distMax = pow(2.0, 32.0);

	// Scan the codebook
	for (int a = 0; a < indexRow; a++) {
		// Initliaze tempeorary distance and reset value of temperary distance after processing 1 row of Codebook
		double distance = 0.0;
		for (int b = 0; b < indexCol; b++) {
			distance = distance + pow(trainingSet.at<double>(i, b) - codebook.at<double>(a,b), 2.0);
		}
		//distance = distance / indexCol;
		// Find the minimum distance and update the index respectively
		if (distance < distMax) {
			distMax = distance;
			indexK = a;
		}
	}

	return indexK;
}

/**
* @brief  Update the codebook using container_cb
* @param
* @retval
*/
int updateCodebook(Mat container_cb, Mat &codebook, Mat count_container)
{
	int indexRow = container_cb.rows;
	int indexCol = container_cb.cols;

	for (int i = 0; i < indexRow; i++) {
		Mat temp(1, indexCol, CV_64F, Scalar::all(0));
		for (int j = 0; j < DIMENSION_CODE_VECTOR; j++) {
			temp.at<double>(0, j) = container_cb.at<double>(i, j);
		}

		for (int k = 0; k < DIMENSION_CODE_VECTOR; k++) {
			temp.at<double>(0, k) = temp.at<double>(0, k) / count_container.at<double>(0, i);
		}
		
		// Updating codebook is happened when container_cb has non zero value
		if (countNonZero(temp) != 0) {
			for (int l = 0; l < DIMENSION_CODE_VECTOR; l++)
				codebook.at<double>(i, l) = temp.at<double>(0, l);
		}		
	}

	return 1;
}

/**
* @brief  Update the distortion factor value
* @param
* @retval
*/
double distortion(Mat codebook, Mat indexClosetMatch, Mat trainingSet)
{
	double sol = 0;

	int LengthTrainingSet = trainingSet.rows;
	int n = trainingSet.cols;

	double sum = 0;

	for (int i = 0; i < LengthTrainingSet; i++) {
		for (int j = 0; j < n; j++) {
			sum = sum + pow((codebook.at<double>(
				indexClosetMatch.at<double>(0, i), j)
				- trainingSet.at<double>(i, j)), 2.0);
		}
	}

	return sol = sum / (LengthTrainingSet*n);
}

/**
* @brief  Create compressed image from indexClosetMatch
* @param
* @retval
*/
Mat createCompressedImage(Mat indexClosetMatch, Mat codebook, Mat image)
{
	Mat rst(image.size(), CV_8UC1, Scalar::all(0));

	int indexCM = 0;

	for (int i = 0; i < image.rows; i = i + BLOCK_SIZE) {
		for (int j = 0; j < image.cols; j = j + BLOCK_SIZE) {
			int index = indexClosetMatch.at<double>(0, indexCM);
			int l = 0;

			for (int k = 0; k < BLOCK_SIZE; k++) {
				for (int t = 0; t < BLOCK_SIZE; t++) {
					rst.at<uchar>(i + k, j + t) = codebook.at<double>(index, l);
					l = l + 1;
				}
			}
			indexCM = indexCM + 1;
		}
	}
	return rst;
}