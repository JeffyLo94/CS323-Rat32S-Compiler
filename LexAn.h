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
*		- unordered_map
***********************************************************/

#ifndef LexAn_H
#define LexAn_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>

using namespace std;

class LexAn {
public:

	LexAn();						        //REMEMBER to Populate HashTable(s)
	~LexAn();

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
	void	lexer(ifstream& infile);

	void	setLexeme(string lex);
	void	setToken(string token);

	void	print();								//Prints current lexeme and token value


	//ACCESSORS
	string	getToken();							//Returns token (in case user wants to print to file)
	string	getLexeme();						//Returns lexeme (in case user wants to print to file)


	enum LexType{
		STRING,
		OPERATOR,
		SEPERATOR,
		ENDFILE,
		ERR
	};

	//Enumerate the states/columns to make it easier to read
	enum States {
		letter = 1,
		digit,
		decimal,
		dollar,
		space,
		punct,
		reject = 11,
		spaceReject,
		punctReject
	};

private:
	//Helper Methods
	int			colNum(char ch);
	bool		DFSM(string potentialLex);

	bool		isSeperator(string str);
	bool		isOperator(string str);
	bool		isKeyword(string str);
  
	bool		isPossibleSepChar(char sep);
	bool		isPossibleOpChar(char op);

	//Private Members:
	//int		curState, prevState, curCol, lineLength;
	string	lexeme;
	string	token;
	//char	currentChar;
	//bool	tokenFound;

	//Hash Table of Keywords, Seperators, Operators
	unordered_map <string, string> keywords;
	unordered_map <string, string> seperators;
	unordered_map <string, string> operators;
};

#endif
