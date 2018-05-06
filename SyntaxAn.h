#pragma once
/***********************************************************
*	CPSC323 - RAT18S - Lexical Analyzer Component
* ---------------------------------------------------------
*	California State University, Fullerton (CSUF)
*	CPSC 323 - Spring 2017
*  Prof Song Choi
*  Assignment #2 - Syntax Analyzer
* ---------------------------------------------------------
*	Authors: Andrew Nguyen, Arman Jafarinia, Jeffrey Lo
* ---------------------------------------------------------
*	Description:
*		- Class Header of Syntax Analyzer
***********************************************************/

#ifndef SyntaxAn_H
#define SyntaxAn_H

#include <string>
#include <vector>
#include "LexAn.h"

struct symbolTableEntry {
	string identiferName;
	int memoryLocation;
	string identifierType;
};

class SyntaxAn {
public:

	SyntaxAn(string inputFile, string outputFile);
	~SyntaxAn();

	//Grammar Methods:
	//Jeffrey Syntaxes
	bool Rat18S();
	bool OptFunctionDefinitions();
	bool FunctionDefinitions();
	bool Function();
	bool OptParameterList();
	bool ParameterList();
	bool Parameter();
	//Arman Syntaxes
	bool Qualifier();
	bool Body();
	bool OptDeclarationList();
	bool DeclarationList();
	bool Declaration();
	bool IDs();
	bool StatementList();
	bool Relop();
	bool Condition();
	//Andrew Syntaxes
	bool statement();
	bool assign();
	bool While();
	bool If();
	bool term();
	bool primary();
	bool expression();
	bool expressionPrime();
	bool termPrime();
	bool factor();
	bool empty();
	bool scan();
	bool print();
	bool compound();
	bool Return();

	void PrintSymbolTable(); 
private:
	//Helper Methods:
	void reportErr(string msg);
	void reportLexerResults();
	void symbolTableInsert(symbolTableEntry entry); 
	bool search(string key); 

	vector<symbolTableEntry> symbolTable; 

	//Lexical Analyzer
	LexAn lex;
	ifstream file;
	ofstream outFile;
};

#endif