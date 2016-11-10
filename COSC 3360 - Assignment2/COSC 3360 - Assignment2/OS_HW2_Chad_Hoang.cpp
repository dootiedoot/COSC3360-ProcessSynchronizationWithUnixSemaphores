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
struct Variable							//	Structure of the variable
{
	string variableName;
	int value;
};
vector<Variable> variables;			
vector<string> processes;				//	Array of input variables given by the data file

//	Methods
void ReadFromFile(string codeFileName, string dataFileName);
vector<string> ParseVariablesIntoString(string startingString, string delimiter, string inputString);
vector<int> ParseDataFileIntoInt(string delimiter, string delimiter2, string inputString);

int main(int argc, char* argv[])
{
	//	Read, Evaluate, and Assign variables based in the input .txt file supplied by command argument
	ReadFromFile(argv[1], argv[2]);
	
	return 0;
}

#pragma region ReadFromFile(): Read, Evaluate, and Assign variables based in the input .txt file supplied by command argument
void ReadFromFile(string codeFileName, string dataFileName)
{
	//	input the string argument of the input file
	fstream codeFile(codeFileName);
	fstream dataFile(dataFileName);

	//	Evaluate the input file
	if (codeFile.is_open())
	{
		string currentInputFileLine;

		//	Loop through lines until "input_var" is found
		while (getline(codeFile, currentInputFileLine))
		{			
			if (currentInputFileLine.find("input_var") != std::string::npos)
			{
				//	Cache the parse variables in the string into our variable array
				vector<string> inputVariables = ParseVariablesIntoString(" ", ",", currentInputFileLine);
				for (size_t i = 0; i < inputVariables.size(); i++)
				{
					Variable newVariable;
					variables.push_back(newVariable);
					variables[i].variableName = inputVariables[i];
				}
				break;
			}
		}
		
		//	Fetch next line
		getline(codeFile, currentInputFileLine);

		//	Cache the parse variables in the string into our processes string array
		processes = ParseVariablesIntoString(" ", ",", currentInputFileLine);

		//	Fetch next line
		getline(codeFile, currentInputFileLine);

		//	Evaluate the data file
		if (currentInputFileLine.find("read") != std::string::npos && dataFile.is_open())
		{
			string currentDataFileLine;

			//	Get the first line in the data file
			getline(dataFile, currentDataFileLine);

			//	Cache the parse variables in the string into our variable array
			vector<int> inputVariables = ParseDataFileIntoInt(","," ", currentDataFileLine);
			for (size_t i = 0; i < inputVariables.size(); i++)
				variables[i].value = inputVariables[i];
			
			//	Assign value of 0 for variables that weren't assigned in the data file
			int amountMissing = variables.size() - inputVariables.size();
			for (size_t i = 0; i < amountMissing; i++)
				variables[variables.size() - 1 - i].value = 0;

			dataFile.close();
		}

		// Display
		for (size_t i = 0; i < variables.size(); i++)
			cout << variables[i].variableName << ": " << variables[i].value << endl;
		cout << endl;
	}
}
#pragma endregion

#pragma region ParseVariablesIntoString: parses variables in the given string starting from the delimiter
vector<string> ParseVariablesIntoString(string startingString, string delimiter, string inputString)
{
	vector<string> parsedArray;

	//	Remove startingString and ";" from string so its easier to parse
	size_t foundIndex = inputString.find(startingString);
	inputString.erase(0, foundIndex + 1);
	foundIndex = inputString.find(";");
	if (foundIndex != -1)
		inputString.erase(foundIndex, foundIndex);

	//	Parse variables from current line to string array
	size_t pos;
	string token;
	while ((pos = inputString.find(delimiter)) != string::npos)
	{
		token = inputString.substr(0, pos);
		
		//	Add to variables array
		parsedArray.push_back(token);

		//	Erase previous token so we can parse next delimiter
		inputString.erase(0, pos + delimiter.length());
	}
	//	Add last token to variables array
	parsedArray.push_back(inputString);

	return parsedArray;
}
#pragma endregion

#pragma region ParseDataFileIntoInt: parses variables in the given string starting from the delimiter
vector<int> ParseDataFileIntoInt(string delimiter, string delimiter2, string inputString)
{
	vector<int> parsedArray;

	//	Remove the ";" from string so its easier to parse
	int foundIndex = inputString.find(";");
	if (foundIndex != -1)
		inputString.erase(foundIndex, foundIndex);

	//	Parse variables from current line to string array
	size_t pos;
	int token;
	while ((pos = inputString.find(delimiter)) != string::npos || (pos = inputString.find(delimiter2)) != string::npos)
	{
		token = stoi(inputString.substr(0, pos));

		//	Add to value array
		parsedArray.push_back(token);

		//	Erase previous token so we can parse next delimiter
		inputString.erase(0, pos + delimiter.length());
	}
	//	Add last token to variables array
	parsedArray.push_back(stoi(inputString));

	return parsedArray;
}
#pragma endregion