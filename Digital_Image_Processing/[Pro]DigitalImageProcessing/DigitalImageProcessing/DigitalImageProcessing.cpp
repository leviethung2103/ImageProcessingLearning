// Struct_QLSV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "menu.h"

using namespace std;

int main()
{
	menu();
	while (true)
	{
		processMenu();
	}
	system("pause");
	return 0;
}

