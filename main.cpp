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

using namespace std;

int main() {
	string	line;
	LexAn	magic;

	string fileName;
	ifstream sourceFile;

	ofstream outfile("lexicalReport.txt", std::ofstream::out | std::ofstream::trunc);

	outfile << "--------------------------------------------------------" << endl;
	outfile << "Lexical Anlyzer Project" << endl;
	outfile << "*California State University, Fullerton(CSUF)" << endl;
	outfile << "*	CPSC 323 - Spring 2017" << endl;
	outfile << "* Authors: Jeffrey Lo, Andrew Nguyen, Arman Jafarinia" << endl;
	outfile << "* Prof Song Choi" << endl;
	outfile << "*	Assignment #1 - Lexical Analyzer" << endl;
	outfile << "--------------------------------------------------------" << endl << endl;
	
	do {
		cout << "Please enter source file .txt (type EXIT to quit): ";
		cin >> fileName;
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
			cout << endl << endl << "OUTPUT:" << endl;
			cout << left << setw(20) << "Token:" << setw(20) << "Lexeme:" << endl << endl;
			outfile << endl << endl << "OUTPUT:" << endl;
			outfile << left << setw(20) << "Token:" << setw(20) << "Lexeme:" << endl << endl;

			//Reset File to be read
			sourceFile.clear();
			sourceFile.seekg(0);

			//Lex Analysis Begins Here:
			while (!sourceFile.eof()) {
				magic.lexer(sourceFile);

				//Outputs Results if not at file end
				if (magic.getLexeme() != "EOF") {
					magic.print();
					outfile << setw(20) << magic.getToken() << setw(20) << magic.getLexeme() << endl;
				}
			} //END While

			sourceFile.close();				//Closing source file
		}// END IF - command is not EXIT
	} while( fileName != "EXIT" );


	//Closing output filestream
	outfile.close();

	system("pause");
	return 0;
}