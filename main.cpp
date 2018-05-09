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
#include "LexAn.h"
#include "SyntaxAn.h"

using namespace std;

int main() {
	string	line;

	string fileName;
	ifstream sourceFile;

	//ofstream outfile("lexicalReport.txt", std::ofstream::out | std::ofstream::trunc);

	//outfile << "--------------------------------------------------------" << endl;
	//outfile << "Lexical Anlyzer Project" << endl;
	//outfile << "*California State University, Fullerton(CSUF)" << endl;
	//outfile << "*	CPSC 323 - Spring 2017" << endl;
	//outfile << "* Authors: Jeffrey Lo, Andrew Nguyen, Arman Jafarinia" << endl;
	//outfile << "* Prof Song Choi" << endl;
	//outfile << "*	Assignment #1 - Lexical Analyzer" << endl;
	//outfile << "--------------------------------------------------------" << endl << endl;
	ofstream outfile("SyntaxReport.txt", std::ofstream::out | std::ofstream::app);

	outfile << "--------------------------------------------------------" << endl;
	outfile << "Compiler Project" << endl;
	outfile << "*California State University, Fullerton(CSUF)" << endl;
	outfile << "*	CPSC 323 - Spring 2017" << endl;
	outfile << "* Authors: Jeffrey Lo, Andrew Nguyen, Arman Jafarinia" << endl;
	outfile << "* Prof Song Choi" << endl;
	outfile << "*	Assignment #2 - Syntax Analyzer" << endl;
	outfile << "--------------------------------------------------------" << endl << endl;

	cout << "Please enter source file .txt (type EXIT to quit): ";
	cin >> fileName;
	//fileName = "TestCase3-ICG.txt"; 
	cout << " " << fileName << endl << endl;

	if (fileName != "EXIT") {
		//Open the file which the user entered in
		sourceFile.open(fileName.c_str());

		//Make sure the files were opened properly
		if (!sourceFile.is_open()) {
			cerr << "Unable to open source file " << fileName << ".\nPlease make sure you enter the correct name.";
			exit(1);
		}
		else { // OUTPUT Source Text Code
			cout		<< "Source File Text:" << endl << endl;
			outfile		<< "Source File Text:" << endl << endl;
			while (getline(sourceFile, line)) {
				cout	<< line << endl;
				outfile << line << endl;
			}
		}

		//Reset File to be read
		sourceFile.clear();
		sourceFile.seekg(0);

		sourceFile.close(); //closing source file
		outfile.close();

		//Syntax Analysis begins here:
		SyntaxAn SyntaxChecker(fileName, "SyntaxReport.txt");
		SyntaxChecker.Rat18S();
		//SyntaxChecker.~SyntaxAn(); //Make sure files are closed
		cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~Symbol Table~~~~~~~~~~~~~~~~~~~~~~~~~~";
		outfile << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~Symbol Table~~~~~~~~~~~~~~~~~~~~~~~~~~";
		SyntaxChecker.PrintSymbolTable(); 
		cout << "\n\n~~~~~~~~~~Instruction Table~~~~~~~~~~";
		outfile << "\n\n~~~~~~~~~~Instruction Table~~~~~~~~~~";
		SyntaxChecker.PrintInstrTable();
	}

	//Closing output filestream
	if (outfile.is_open()) {
		outfile.close();
	}

	system("pause");
	return 0;
}