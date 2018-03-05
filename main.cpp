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

	ofstream outfile("lexicalReport.txt", std::ofstream::out | std::ofstream::trunc);

	outfile << "--------------------------------------------------------" << endl;
	outfile << "Lexical Anlyzer Project" << endl;
	outfile << "*California State University, Fullerton(CSUF)" << endl;
	outfile << "*	CPSC 323 - Spring 2017" << endl;
	outfile << "* Prof Song Choi" << endl;
	outfile << "*	Assignment #1 - Lexical Analyzer" << endl;
	outfile << "--------------------------------------------------------" << endl << endl;

	cout << "Please enter source file name to test: ";
	cin >> fileName;
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
		lextest.lexer(line, outfile);
	}

	//Closing file streams
	sourceFile.close();
	outfile.close();

	system("pause");
	return 0;
}