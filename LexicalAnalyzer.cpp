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
*	LexicalAnalyzer.cpp
*
*	CLASS LexicalAnalyzer
*		- This class contains a lexer function which reads
*		  from a file and determines lexeme and token
***********************************************************/

//Read OverallStructure.txt for Guide

#include "LexicalAnalyzer.h"

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

//This is the transistion table for all both finite state machines. 
//This should be able to tell us if it is an Integer, Identifier/KEYWORD, or Real
//The first row isn't used by this program and is there for clarity
int transistionTable[14][7] =
{ 
	{ 0,	letter,	digit,	decimal,	dollar,	space,	punct },
	{ 1,	3,		2,		11,			11,		12,		13	},	//Starting State
	{ 2,	11,		5,		4,			11,		12,		13	},	//Accept Integer
	{ 3,	8,		7,		11,			6,		12,		13	},	//Accepting Identifier
	{ 4,	11,		9,		11,			11,		12,		13	},	//Accept Integer
	{ 5,	11,		5,		4,			11,		12,		13	},
	{ 6,	11,		11,		11,			11,		12,		13	},	//Accept Identifier
	{ 7,	8,		7,		11,			6,		12,		13	},
	{ 8,	8,		7,		11,			6,		12,		13	},	//Accept Identifier/KEYWORD - Check for KEYWORD if last state
	{ 9,	11,		10,		11,			11,		12,		13	},	//Accept Real
	{ 10,	11,		10,		11,			11,		12,		13	},
	{ 11,	11,		11,		11,			11,		12,		13	},
	{ 12,	12,		12,		12,			12,		12,		12	},
	{ 13,	11,		11,		11,			11,		12,		13	}										
};

LexicalAnalyzer::LexicalAnalyzer() {
	//Initializing Hashtable of special case lexemes
	//OPERATORs:
	specialLexs.insert({
		//OPERATORs:
		{ "=", "OPERATOR" },
		{ "+", "OPERATOR" },
		{ "-", "OPERATOR" },
		{ "*", "OPERATOR" },
		{ "/", "OPERATOR" },
		{ "==", "OPERATOR" },
		{ "^=", "OPERATOR" },
		{ "<", "OPERATOR" },
		{ ">", "OPERATOR" },
		{ "=<", "OPERATOR" },
		{ "=>", "OPERATOR" },
		//SEPERATORs
		{ "%%", "SEPERATOR" },
		{ ":", "SEPERATOR" },
		{ "[", "SEPERATOR" },
		{ "]", "SEPERATOR" },
		{ ",", "SEPERATOR" },
		{ "{", "SEPERATOR" },
		{ "}", "SEPERATOR" },
		{ "(", "SEPERATOR" },
		{ ")", "SEPERATOR" },
		{ ";", "SEPERATOR" },
		//KEYWORDs
		{ "function", "KEYWORD" },
		{ "int", "KEYWORD" },
		{ "boolean", "KEYWORD" },
		{ "real", "KEYWORD" },
		{ "if", "KEYWORD" },
		{ "else", "KEYWORD" },
		{ "endif", "KEYWORD" },
		{ "return", "KEYWORD" },
		{ "put", "KEYWORD" },
		{ "get", "KEYWORD" },
		{ "while", "KEYWORD" },
		{ "true", "KEYWORD" },
		{ "false", "KEYWORD" },
	});
}

LexicalAnalyzer::~LexicalAnalyzer() {

}

/////////////////////////////////////////////////////////////////////////
//FUNCTION: Lexer
//USE:	Read the line from the source file and run each character through
//		the transistion table. It will then find out if the token is 
//		a ketword, integer, real, etc...
//@param line - (string) line taken from the sourcefile
////////////////////////////////////////////////////////////////////////
int LexicalAnalyzer::lexer(string line) {
	//Starting state
	int curState = 1;
	//This will be used to go back a state if it is a space, tab, OPERATOR, etc...
	int prevState = 1;
	//Get the length of the line.  This will be
	int lineLength = line.length();
	// Column variable will be used for the transistion table
	int curCol = 0;
	//No use for this variable yet. Might use it to see if we found token yet or not.
	currentChar = ' ';
	token = "";

	for (int i = 0; i < lineLength;) {
		//Get the character
		currentChar = line[i];
		curCol = colNum(currentChar);
		prevState = curState;
		bool tokenFound = false;
		//Check the transistion table to see what state it currently is after getting the next column
		curState = transistionTable[curState][curCol];

		//Still working on these down here. Will update with more info
		//Currently it is creating the token one character at a time
		if (curState != reject && curState != spaceReject && curState != punctReject && i != lineLength) {
			token += currentChar;
			i++;
		}
		//This program can find most tokens with a space after it. Will need fine tuning and testing to complete
		else if (curState == spaceReject /*&& curState != punctReject*/) {
			unordered_map <string, string>::iterator itr = specialLexs.find(token);

			if (itr != specialLexs.end()) {
				cout << itr->first << "\t\t" << itr->second << endl;
				tokenFound = true;
			}
			else if (tokenFound == false) {
				cout << token << "\t\t" << getLexemeName(prevState) << endl;
				tokenFound = true;
			}

			token = "";
			curState = 1;
			i++;
		}
		//Operators/Separator isn't working yet. Im doing a few tests here to see which will work best
		else if (curState == punctReject) {
			if (prevState != punctReject && prevState != 1)
			{
				cout << token << "\t\t" << getLexemeName(curState) << endl;
				token = currentChar;
				tokenFound = true;
				i++;
			}
			else {
				if (curState == punctReject && prevState == punctReject)
				{
					token += currentChar;
					unordered_map <string, string>::iterator itr = specialLexs.find(token);

					if (itr != specialLexs.end()) {
						cout << itr->first << "\t\t" << itr->second << endl;
						token = "";
						i++;
						curState = 1;
						tokenFound = true;
					}
					else {
						cout << token << "\t\t" << getLexemeName(curState) << endl;
						token = "";
						curState = 1;
						tokenFound = true;
						i++;
					}
				}
				else {
					token += currentChar;
					i++;

					unordered_map <string, string>::iterator itr = specialLexs.find(token);

					if (itr != specialLexs.end()) {
						cout << itr->first << "\t\t" << itr->second << endl;
						token = "";
						i++;
						curState = 1;
						tokenFound = true;
					}
				}
			}
		}
		else if (prevState != spaceReject && prevState != curState && tokenFound == false) {
			
		}
		if (i == lineLength && tokenFound == false) {
			cout << token << "\t\t" << getLexemeName(curState) << endl;
			token = "";
			curState = 1;
			tokenFound = true;
		}
	}
	return 0;
}

int LexicalAnalyzer::colNum(char ch) {
	if(isdigit(ch)) {
		return digit;
	}
	else if(isalpha(ch)) {
		return letter;
	}
	else if(ch == '$') {
		return dollar;
	}
	else if (ch == '.') {
		return decimal;
	}
	else if(isspace(ch)) {
		return space;
	}
	else if (ispunct(ch)) {
		return punct;
	}
}

string LexicalAnalyzer::getLexemeName(int state) {
	if (state == 2 || state == 4) {
		return "INTEGER";
	}
	else if (state == 6 || state == 3) {
		return "IDENTIFIER";
	}
	else if (state == 8) {
		return "IDENTIFIER";
	}
	else if (state == 9) {
		return "REAL";
	}
	else if (state == 13) {
		return "OPERATOR";
	}
	else
		return "UNKNOWN";
}

//string LexicalAnalyzer::getLexeme() {
//
//}
//
//string LexicalAnalyzer::getToken() {
//
//}
//
//bool LexicalAnalyzer::isKEYWORD(string KEYWORD) {
//
//}
//
//bool LexicalAnalyzer::isOPERATOR(char ch) {
//
//}
//
//bool LexicalAnalyzer::isSEPERATOR(char ch) {
//
//}
//
//bool LexicalAnalyzer::idDFSM(char ch) {
//
//}
//
//int LexicalAnalyzer::DFSM(char state) {
//	
//}
//
//bool LexicalAnalyzer::realDFSM(char ch) {
//
//}