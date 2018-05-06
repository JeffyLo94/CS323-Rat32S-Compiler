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
	string identifierName;
	int memoryLocation;
	string identifierType;
};

struct instrTableEntry {
	int address = 0;
	string op = "";
	string oprnd = "";
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
	void PrintInstrTable();
private:
	//Helper Methods:
	void reportErr(string msg);
	void reportLexerResults();

	void symbolTableInsert(symbolTableEntry entry); 
	bool checkSymbolTable(string);
	bool search(string key); 
	int instr_address = 1;
	string curQualifier = "";
	int mem_location = 2001;

	void gen_instr(string, string);

	symbolTableEntry newEntry;
	vector<symbolTableEntry> symbolTable;

	instrTableEntry newInstrEntry;
	vector<instrTableEntry> instrTable;

	bool symbolFound = false;
	string get_address(string);

	//Lexical Analyzer
	LexAn lex;
	ifstream file;
	ofstream outFile;
};

#endif