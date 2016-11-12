#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

/*
	Assignment 2
	COSC 3360 - Fundamentals of Operating Systems
	University of Houston
	Chad Hoang
	1176413
*/

// Variables
struct Variable								//	Structure of the variable
{
	string variableName;
	int value;
};
vector<Variable> variables;			
struct Process								//	Structure of the process
{
	string processName;
	vector<string> instructions;
};
vector<Process> processes;					//	Array of processes given by the input file
vector<string> mainInstructions;			//	Array of instructions executed by the the main process
const char * expressionToParse;

//	Methods
#pragma region Method initalization
void ReadFromFile(string codeFileName, string dataFileName);
vector<string> ParseVariablesIntoString(string startingString, string delimiter, string inputString);
vector<int> ParseDataFileIntoInt(string delimiter, string delimiter2, string inputString);
string RemoveSpaces(string input);
float number();
float factor();
float term();
float ParseExpression();
char* ToCharArray(string input);
string ParseExpressionVariables(string inputString);
#pragma endregion

int main(int argc, char* argv[])
{
	cout << endl;
	
	//	Read, Evaluate, and Assign variables based in the input .txt file supplied by command argument
	ReadFromFile(argv[1], argv[2]);

	//	Parse all instruction expressions for each process
	for (size_t i = 0; i < processes.size(); i++)
	{
		for (size_t j = 0; j < processes[i].instructions.size(); j++)
		{
			processes[i].instructions[j] = ParseExpressionVariables(processes[i].instructions[j]);

			expressionToParse = ToCharArray(processes[i].instructions[j]);
			float result = ParseExpression();
			cout << "Process " << processes[i].processName << " instruction " << processes[i].instructions[j] << " equals " << result << endl << endl;
		}
	}

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
		vector<string> inputProcesses = ParseVariablesIntoString(" ", ",", currentInputFileLine);
		for (size_t i = 0; i < inputProcesses.size(); i++)
		{
			Process newProcess;
			processes.push_back(newProcess);
			processes[i].processName = inputProcesses[i];
		}

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
			cout << "Variable " << variables[i].variableName << " = " << variables[i].value << endl;
		cout << endl;

		// Display
		for (size_t i = 0; i < processes.size(); i++)
			cout << "Process " << processes[i].processName << endl;
		cout << endl;

		//	Loop until "write" is found
		while (true)
		{
			//	Fetch next line
			getline(codeFile, currentInputFileLine);

			//	if the current line contains "write" break loop
			if (currentInputFileLine.find("write") != std::string::npos)
				break;

			//	else if, next line is cobegin. Loop until coend is found and cache all instructions between them
			else if (currentInputFileLine.find("cobegin") != std::string::npos)
			{
				//cout << "cobegin" << endl;
				
				while (true)
				{
					//	Fetch next line
					getline(codeFile, currentInputFileLine);

					//	if "coend" is found, break loop
					if (currentInputFileLine.find("coend") != std::string::npos)
					{
						//cout << "coend" << endl;
						break;
					}
					//	else if nested "cobegin" is found, repeat loop
					/*else if (currentInputFileLine.find("cobegin") != std::string::npos)
					{
						cout << "cobegin" << endl;
						
						while (true)
						{
							//	Fetch next line
							getline(codeFile, currentInputFileLine);

							//	if "coend" is found, break loop
							if (currentInputFileLine.find("coend") != std::string::npos)
							{
								cout << "coend" << endl;
								break;
							}

							//	Remove the spaces from string so its easier to parse
							currentInputFileLine = removeSpaces(currentInputFileLine);
							//	Remove the ";" from string so its easier to parse
							size_t foundIndex = currentInputFileLine.find(";");
							if (foundIndex != -1)
								currentInputFileLine.erase(foundIndex, foundIndex);

							//	Add line to instructions array
							concurrentInstructions.push_back(currentInputFileLine);
							cout << concurrentInstructions.back() << " " << endl;
						}
					}*/

					//	Remove the spaces from string so its easier to parse
					currentInputFileLine = RemoveSpaces(currentInputFileLine);
					//	Remove the ";" from string so its easier to parse
					size_t pos = currentInputFileLine.find(";");
					if (pos != -1)
						currentInputFileLine.erase(pos, pos);

					//	Find the processName and cache its position
					size_t equalPos = currentInputFileLine.find("=");
					string processName = currentInputFileLine.substr(0, equalPos);
					//	Remove anything before the "=" sign so it just leaves the expression
					currentInputFileLine.erase(0, equalPos + 1);

					//	Add line to instructions array of the specific process
					for (size_t i = 0; i < processes.size(); i++)
					{
						if (processName.find(processes[i].processName) != std::string::npos)
						{
							processes[i].instructions.push_back(currentInputFileLine);
							break;
						}
					}
				}
			}
			//	else, the instruction is most likey a main instruction and will be cached
			else
			{
				//	Remove the spaces from string so its easier to parse
				currentInputFileLine = RemoveSpaces(currentInputFileLine);
				//	Remove the ";" from string so its easier to parse
				size_t foundIndex = currentInputFileLine.find(";");
				if (foundIndex != -1)
					currentInputFileLine.erase(foundIndex, foundIndex);

				//	Add line to instructions array
				mainInstructions.push_back(currentInputFileLine);
				cout << mainInstructions.back() << " " << endl;
			}
		}

		// Display
		for (size_t i = 0; i < processes.size(); i++)
		{
			for (size_t j = 0; j < processes[i].instructions.size(); i++)
			{
				cout << "Process " << processes[i].processName << " has instruction: " << processes[i].instructions[j] << endl;
			}
		}
		cout << endl;

		//cout << currentInputFileLine << endl;
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

#pragma region RemoveSpaces(): removes spaces in a string
string RemoveSpaces(string input)
{
	//	Remove the spaces before the string
	int foundIndex = input.find("p");
	input.erase(0, foundIndex);
	
	//	Remove starting spaces from string so its easier to parse
	int length = input.length();
	for (int i = 0; i < length; i++) 
	{
		if (input[i] == ' ')
		{
			input.erase(i, 1);
			length--;
			i--;
		}
	}

	return input;
}
#pragma endregion

#pragma region Arithmetic solving methods
char peek()
{
	return *expressionToParse;
}

char get()
{
	return *expressionToParse++;
}

float ParseExpression();

float number()
{
	int result = get() - '0';
	while (peek() >= '0' && peek() <= '9')
	{
		result = 10 * result + get() - '0';
	}
	return result;
}

float factor()
{
	if (peek() >= '0' && peek() <= '9')
		return number();
	else if (peek() == '(')
	{
		get(); // '('
		float result = ParseExpression();
		get(); // ')'
		return result;
	}
	else if (peek() == '-')
	{
		get();
		return -factor();
	}
	return 0; // error
}

float term()
{
	float result = factor();
	while (peek() == '*' || peek() == '/')
		if (get() == '*')
			result *= factor();
		else
			result /= factor();
	return result;
}

float ParseExpression()
{
	float result = term();
	while (peek() == '+' || peek() == '-')
		if (get() == '+')
			result += term();
		else
			result -= term();
	return result;
}
#pragma endregion

#pragma region ToCharArray(): converts string to char array
char* ToCharArray(string input)
{
	//	Convert string to char array
	char *stringToCharArray = new char[input.length() + 1];
	strcpy(stringToCharArray, input.c_str());

	return stringToCharArray;
}
#pragma endregion

#pragma region ParseExpressionVariables: parses variables in the given expression string
string ParseExpressionVariables(string inputString)
{
	for (size_t i = 0; i < inputString.length(); i++)
	{
		for (size_t j = 0; j < variables.size(); j++)
		{
			size_t pos = inputString.find(variables[j].variableName);
			if (pos != -1)
			{
				//cout << variables[j].variableName << " found from " << pos << " to " << pos + variables[j].variableName.length() << endl;
				inputString.replace(pos, variables[j].variableName.length(), to_string(variables[j].value));
				//cout << variables[j].variableName << " found from " << pos << " to " << pos + variables[j].variableName.length() << endl;
				//inputString.replace(pos, pos + variables[j].variableName.length(), to_string(variables[j].value));
				//cout << "Updated expression: " << inputString << endl;
				//cout << "length: " << inputString.length() << endl;
				break;
			}
		}
	}

	return inputString;
}
#pragma endregion