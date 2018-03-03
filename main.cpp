/***********************************************************
 *	CPSC323 - RAT32S - Lexical Analyzer Component
 * ---------------------------------------------------------
 *	California State University, Fullerton (CSUF)
 *	CPSC 323 - Spring 2017
 *  Prof Song Choi
 *  Assignment #1 - Lexical Analyzer
 * ---------------------------------------------------------
 *	Authors: Andrew Nguyen, Arman Jafarinia, Jeffrey Lo
 * ---------------------------------------------------------
 *	Description:
 *		- Tests Lexical Analyzer
 *	INPUTS
 *		- Input Filename
 *	OUTPUTS
 *		- Console
 *		- Analysis.txt
 ***********************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.h"

using namespace std;

int main() {
	string line;
	LexicalAnalyzer lextest;

	string fileName;
	ifstream sourceFile;

	string reportName;
	ofstream lexicalReport;

	cout << "Please enter source file name to test: ";
	//cin >> fileName;
	fileName = "source.txt";
	cout << " " << fileName << endl << endl;
	
	//Open the file which the user entered in
	sourceFile.open(fileName);

	//Make sure the files were opened properly
	if (!sourceFile) {
		cerr << "Unable to open source file " << fileName << ".\nPlease make sure you enter the correct name.";
		system("pasuse");
		return 0;
	}

	//This will read the source file one line at a time and passes it to the lexer function
	while (getline(sourceFile, line)) {
		lextest.lexer(line);
	}

	//Closing file streams
	sourceFile.close();
	lexicalReport.close();

	system("pause");
	return 0;
}