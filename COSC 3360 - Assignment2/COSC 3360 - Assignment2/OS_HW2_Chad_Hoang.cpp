#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>

using namespace std;

/*
Assignment 2
COSC 3360 - Fundamentals of Operating Systems
University of Houston
Chad Hoang
1176413
*/

// Variables
int *inputVariables;				//	Array of input variables given by the data file

int main(int argc, char* argv[])
{
	//	Read, Evaluate, and Assign variables based in the input .txt file supplied by command argument
	ReadFromFile(argv[1], argv[2]);
	
	return 0;
}

void ReadFromFile(string codeFileName, string dataFileName)
{
	//	input the string argument of the input file
	fstream codeFile(codeFileName);
	fstream dataFile(dataFileName);

	//	Evaluate the input file
	if (codeFile.is_open())
	{

	}
}