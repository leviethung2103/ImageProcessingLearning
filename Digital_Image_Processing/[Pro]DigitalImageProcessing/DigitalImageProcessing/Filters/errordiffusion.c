
#include "errorDiffusion.h"
	
using namespace std;
using namespace cv;


void errorDiffusion(void) 
{
	errDiff e;

	// Show the gray scale images
	Mat image = e.loadImage();

	// Display Image
	e.displayImage("Image",image);

	// Create padding matrix
	Mat zeroMatrix = e.zeropadding(image);

	// Create a mask image
	image = e.maskImage(zeroMatrix,image);

	//Error Diffusion Process
	image  = e.diff(image);

	// Show the resutl
	e.displayImage("Result",image);

	waitKey(0);
}
