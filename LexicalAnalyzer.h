#pragma once
/***********************************************************
*	CPSC323 - RAT32S - Lexical Analyzer Component
* ---------------------------------------------------------
*	California State University, Fullerton (CSUF)
*	CPSC 323 - Spring 2017
*	Prof Song Choi
*	Assignment #1 - Lexical Analyzer
* ---------------------------------------------------------
*	Authors: Andrew Nguyen, Arman Jafarinia, Jeffrey Lo
* ---------------------------------------------------------
*	LexicalAnalyzer.h
*
*	Description:
*		Header file for LexicalAnalyzer Class
*	C++ Dependencies:
*		- string
*		- iostream
*		- fstream
*		- iomanip
*		POSSIBLY MORE <---- comment out when fixed
***********************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>

using namespace std;

class LexicalAnalyzer {
public:

	LexicalAnalyzer();						        //REMEMBER to Populate HashTable(s)
	~LexicalAnalyzer();

	int lexer(string);					//Where the Magic Happens
										/*	FUNCTION lexer
										*	1. gets char from file
										*		- until sees seperator, operator, whitespace, eof
										*		- if is is sep, op, or whitespace -> flag to break loop & set character into string & unget
										*		- otherwise create identifier string
										*	2. Check if string is an Seperator, Operator, integer, Real, keyword, or identifier & SET appropriate values for Token and Lexeme
										*		- This can be broken up into a "classification" function that returns a number
										*			- DFSM functions can be called in here
										*		- number can be used in a switch statement to set appropriate lexeme and token private members
										*		- need to add special case for seperator "%%" -> needs to file.peek()
										*		- WARNING - may be more special cases
										*/

	void print();								    // Prints to current lexeme and token value to console

													//ACCESSORS
	string	getToken();								//Returns token (in case user wants to print to file)
	string	getLexeme();							//Returns lexeme (in case user wants to print to file)

													//MUTATORS
													//void	setToken(string nT);					//Not sure if these are necessary
													//void	setLexeme(string nL);				    //Not sure if these are necessary

private:
	//Helper Methods
	bool    integerDFSM(const string);		        //REMEMBER to include comment diagram of DFSM when create
	bool	realDFSM(const string);				    //REMEMBER to include comment diagram of DFSM when create
	bool	idDFSM(const string);	                //REMEMBER to include comment diagram of DFSM when create


	bool	isSeperator(char);
	bool	isOperator(char);
	bool	isKeyword(string);

	//Private Members:
	char	currentChar;							    //character
	string	lexeme;
	string	token;

	unordered_map <string, string> specialLexs;		//Hash Table of Keywords, Seperators, Operators
};