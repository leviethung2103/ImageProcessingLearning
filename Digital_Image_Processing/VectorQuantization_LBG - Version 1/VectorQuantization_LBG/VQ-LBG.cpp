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
	// use current time as seed for random generator
	srand((unsigned)time(0));

	// Calculate the total pixels of image
	int NoPixelsImage = image.rows*image.cols;

	//  n = length of training set = total blockds in one image
	int LengthTrainingSet = NoPixelsImage / LENGTH_CODE_VECTOR;

	// Create training set = 4096x64
	Mat trainingSet(LengthTrainingSet, LENGTH_CODE_VECTOR, CV_64F, Scalar::all(0));

	// Create a codebook with size 64x64
	Mat codeBook(LENGTH_CODE_BOOK, LENGTH_CODE_VECTOR, CV_64F, Scalar::all(0));

	// Initilize Training Set
	initTrainingSet(image,trainingSet);

	// Initilize Codebook 64x64
	initCodebook(codeBook, trainingSet,LengthTrainingSet);

	// then trainingSet and codebook 

	// Create region to store classified traning vector
	Mat Region(LengthTrainingSet,1,CV_64F,Scalar::all(0));

	// Count Number of Element appears in Region
	Mat countElementInRegion(LENGTH_CODE_BOOK, 1, CV_64F, Scalar::all(0));

	Mat sumElementInRegion(LENGTH_CODE_BOOK,LENGTH_CODE_VECTOR,CV_64F,Scalar::all(0));

	/******************* End Decleration *********************************/

	double distortionCondition = 1.0;

	double distortionPrev = 1.0;
	double distortion = 1.0;

	cout << "The Distortion Thereshold is : " << THRESHOLD << endl;


	while (distortionCondition > THRESHOLD) {
	// Find the minimum distortion partion Si | i = 1,..., Nc 
		
	/*
	 *	Applying the Euclidean distance to calculate distance between input vectors and codewords
	 */

	distortionPrev = distortion;

	int indexCodebook = 0;
	// For-loop is used to scan the trainingSet 
	for (int i = 0; i < trainingSet.rows; i++ ) {
		// Set the max value for distance
		double distanceMin = pow(9.99, 62.0);
		
		double error = 0.0;
		for (int j = 0; j < LENGTH_CODE_BOOK; j++) { // Scan row codebook
			// Initliaze tempeorary distance and reset value of temperary distance after processing 1 row of Codebook
			double distance = 0.0;
			for (int k = 0; k < LENGTH_CODE_VECTOR ; k++) { // scan col codebook
				error = trainingSet.at<double>(i,k) - codeBook.at<double>(j,k);
				distance = distance + pow(error,2.0);
			}
			distance = distance / LENGTH_CODE_VECTOR;
			// Find the mimimum value of distance 
			if (distance < distanceMin) {
				distanceMin = distance;
				indexCodebook = j;
			}
		}
		Region.at<double>(i,0) = indexCodebook; 
		countElementInRegion.at<double>(indexCodebook,0)++ ;		
	}
	//double sumabc = cv::sum(countElementInRegion)[0];

	/*
	 *	Find the optimal reproduction alphabet 
	 */

	/* Scan the trainingset, sum of input vectors in their region */

	for (int i2 = 0; i2 < LengthTrainingSet; i2++) {
		// Jump the Region 34
		int indexTemp = Region.at<double>(i2,0);
		// Scan the sumElementRegion
		for (int j2 = 0; j2 < LENGTH_CODE_VECTOR; j2++) {
				// Go the Region with specific indexTemp
				// Copy the value of training to sumElementInRegion
			sumElementInRegion.at<double>(indexTemp, j2) += trainingSet.at<double>(i2, j2);
		}
	}
	//double sumabc = cv::sum(trainingSet)[0];
	//double sumxyz = cv::sum(sumElementInRegion)[0];



	/*
	 *	Update codebook with the new value yi or finding Euclidean center of gravity or centroid
	 */
	for (int i3 = 0; i3 < LENGTH_CODE_BOOK; i3++) {
		int countTemp = countElementInRegion.at<double>(i3, 0);
		if (countTemp != 0) {
			for (int j3 = 0; j3 < LENGTH_CODE_VECTOR; j3++) {
				codeBook.at<double>(i3, j3) = sumElementInRegion.at<double>(i3, j3) / countElementInRegion.at<double>(i3, 0);
			}
		}
	}

   /*
	*	Commpute the distortion D
	*/
	distortion = calculateDistortion(Region, trainingSet, codeBook);

	distortionCondition = abs((distortionPrev - distortion)) / (distortionPrev);

	cout << "The Distortion is : " << distortionCondition << endl;


	} 

	// Create the compressed image using codebook and indexClosetMatch
	Mat result = createCompressedImage(Region,codeBook, image);
	imwrite("Result_Lena.jpg",result);
	// Result
	namedWindow("Modified Image", WINDOW_AUTOSIZE);
	imshow("Modified Image", result);

	return 1;
}

/**
* @brief  Initialize Training Set based on image and Number of vectors
* @param  image,traingSet
* @retval void
*/
int initTrainingSet(Mat image, Mat &trainingSet)
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
					trainingSet.at<double>(indexRow, indexCol) = image.at<uchar>(i2 + i1, j2 + j1);
					indexCol++;
				}
			}
			// Reset indexCol to start a new mission
			indexCol = 0;
			indexRow++;
		}
	}
	return 1;
}

/**
* @brief  Initialize Codebook by randomly picking codevector from trainingSet
* @brief  Select 64 codevectors at random to be the inital codebook
* @param
* @retval void
*/
int initCodebook(Mat &Codebook, Mat trainingSet, int LengthTrainingSet)
{
	// For testing
	Mat test(LENGTH_CODE_BOOK, 1, CV_64F, Scalar::all(0));

	for (int i = 0; i < LENGTH_CODE_BOOK; i++) {
		int random_number = rand() % LengthTrainingSet;
		for (int j = 0; j < LENGTH_CODE_VECTOR; j++) {
			Codebook.at<double>(i, j) = trainingSet.at<double>(random_number, j);
			
		}
		test.at<double>(i, 0) = random_number;
	}
	return 1;
}

/**
* @brief  Create compressed image from Region
* @param  image, codebook. region
* @retval rst
*/
Mat createCompressedImage(Mat Region, Mat codebook, Mat image)
{
	Mat rst(image.size(), CV_64F,Scalar::all(0));


	// Finally, we already had region, codebook and trainingset
	
	// Replace the input vectors with suitable codevector

	// Scan the trainingset 

	int indexTemp = 0;

	for (int i = 0; i < rst.rows; i = i+ BLOCK_SIZE) {
		for (int j = 0; j < rst.cols; j = j+BLOCK_SIZE) {
			// Scan col of codevector
			int k = 0;
			for (int i2 = 0; i2 < BLOCK_SIZE; i2++) {
				for (int j2 = 0; j2 < BLOCK_SIZE; j2++) {
					// Get the index from Region index = 1,2,.., Nc 
					int index = Region.at<double>(indexTemp,0);

					rst.at<double>(i+i2,j+j2) = codebook.at<double>(index,k);
					k++;
				}
			}
			indexTemp++;
		}
	}

	rst.convertTo(rst, CV_8U);
	return rst;
}



/**
* @brief  Caculate the distortion D
* @param
* @retval
*/
double calculateDistortion(Mat Region, Mat trainingSet, Mat codeBook)
{
	double distance = 0.0;
	double totalDistance = 0.0;
	double distortion = 0.0;
	double error = 0.0;

	for (int i = 0; i < trainingSet.rows; i++ ){
		for (int j = 0; j < trainingSet.cols; j++) {
			error = trainingSet.at<double>(i,j) - codeBook.at<double>(Region.at<double>(i,0),j);
			distance = distance + pow(error, 2.0);
		}
		totalDistance += distance;
	}
	return distortion = totalDistance/LENGTH_CODE_VECTOR;
}
