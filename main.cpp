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

#define operatorLength 8
#define separatorLength 10

using namespace std;

string keywords[13] = { "function","int","boolean","real","if","else","endif","return","put","get","while","true","false" };
char operators[] = "+-*/=^><";
char separators[] = "%:[],{}();";

enum States {
	id,
	op,
	keyword,
	separator,
	integer,
	real
};

int lexer(char, char[], int);

int main() {
	char ch, buffer[20];
	int state = 0;
	string expression;

	LexicalAnalyzer lextest;

	string fileName;
	ifstream sourceFile;

	string reportName;
	ofstream lexicalReport;

	cout << "Please enter source file name to test: ";
	cin >> fileName;

	sourceFile.open(fileName);

	if (!sourceFile) {
		cerr << "Unable to open source file " << fileName << ".\nPlease make sure you enter the correct name.";
		return 0;
	}

	while (!sourceFile.eof()) {
		getline(sourceFile, expression);
		cout << lextest.lexer(expression);
	}

	sourceFile.close();
	lexicalReport.close();

return 0;
}