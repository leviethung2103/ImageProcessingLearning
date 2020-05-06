#include "dotDiffusion.h"

void dotDiffusion() 
{
	// Using dot Class
	dot d;

	// Load the gray scale image
	Mat image = d.loadImage();

	// Display Gray scale Image
	d.displayImage("Image",image);

	//Convert image to double precision	
	d.convert2Double(image);

	//d.extendCM(image);

	// Create a mask image
	//image = d.maskImage(image);

	//	Dot diffusion proposed by Knuth
	Mat result = d.runDotDiffusion(image);

	d.displayImage("Dot Diffusion", result);


	// Waitkey
	waitKey(0);
}