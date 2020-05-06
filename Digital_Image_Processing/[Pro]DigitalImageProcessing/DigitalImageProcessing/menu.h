#pragma once

#include "iostream"
#include "Filters/invert.h"
#include "Filters/orderedDithering.h"
#include "Filters/histogram.c"
#include "Filters/errorDiffusion.c"
#include "Filters/gaussian.c"
#include "Filters/dotDiffusion.c"

using namespace std;

void menu() {
	cout << "1. Invert Image " << endl;
	cout << "2. Ordered Dithering " << endl;
	cout << "3. Histogram Equalization " << endl;
	cout << "4. Gaussian Filtering " << endl;
	cout << "5. Error Diffusion" << endl;
	cout << "6. Dot Diffusion" << endl;
	cout << "7. Exit program " << endl;
}

int selectMenu()
{
	int option = 0;
	int firstNumberSelection = 1;
	int lastNumberSelection = 7;

	cout << "\n\nPlease select menu: ";
	cin >> option;

	if (option < firstNumberSelection || option > lastNumberSelection) {
		cout << "You've selected incorrect number. Let's choose again !" << endl;
		return selectMenu();
	}
	else
		return option;
}

void processMenu()
{
	int option = selectMenu();

	switch (option)
	{
	case 1:
		invertImage();
		break;
	case 2:
		orderedDithering();
		break;
	case 3:
		histogram();
		break;
	case 4:
		gaussian();
		break;
	case 5:
		errorDiffusion();
		break;
	case 6:
		dotDiffusion();
		break;
	case 7:
		cout << "Thank you for using my program" << endl;
		exit(1);
		break;

	}
}