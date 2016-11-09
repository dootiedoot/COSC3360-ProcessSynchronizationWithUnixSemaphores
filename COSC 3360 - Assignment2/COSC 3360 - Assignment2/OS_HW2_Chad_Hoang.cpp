#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
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
vector<string> variables;				//	Array of input variables given by the data file
vector<string> processes;					//	Array of input variables given by the data file

//	Methods
void ReadFromFile(string codeFileName, string dataFileName);
vector<string> ParseVariablesInString(string startingString, string inputString);

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
		//	Get the first line
		string currentLine;
		
		//	Loop through lines until "input_var" is found
		while (getline(codeFile, currentLine))
		{			
			if (currentLine.find("input_var") != std::string::npos)
			{
				//	Cache the parse variables in the string into our variables string array
				variables = ParseVariablesInString(" ", currentLine);
				break;
			}
		}
		
		//	Fetch next line
		getline(codeFile, currentLine);

		//	Cache the parse variables in the string into our processes string array
		processes = ParseVariablesInString(" ", currentLine);
	}
}

#pragma region ParseVariablesInString: parses variables in the given string starting from the delimiter
vector<string> ParseVariablesInString(string startingString, string inputString)
{
	vector<string> parsedArray;

	//	Remove "input_var" and ";" from string so its easier to parse & remove
	size_t foundIndex = inputString.find(startingString);
	inputString.erase(0, foundIndex + 1);
	foundIndex = inputString.find(";");
	if (foundIndex != -1)
		inputString.erase(foundIndex, foundIndex);

	//	Parse variables from current line to string array
	string delimiter = ",";
	size_t pos = 0;
	string token;
	int count = 0;
	while ((pos = inputString.find(delimiter)) != string::npos)
	{
		token = inputString.substr(0, pos);

		//	Add to variables array
		parsedArray.push_back(token);

		//	Erase previous token so we can parse next delimiter
		inputString.erase(0, pos + delimiter.length());
	}
	//	Add last token to variables array
	parsedArray.push_back(token);

	// Display
	for (size_t i = 0; i < parsedArray.size(); i++)
		cout << parsedArray[i] << " ";
	cout << endl;

	return parsedArray;
}
#pragma endregion