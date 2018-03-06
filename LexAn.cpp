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
#include "LexAn.h"

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


//Constructor
LexAn::LexAn() {
	//Initializing Hashtable of special case lexemes
	//OPERATORs:
	operators.insert({
		//OPERATORs:
		{ "=",	"OPERATOR" },
		{ "+",	"OPERATOR" },
		{ "-",	"OPERATOR" },
		{ "*",	"OPERATOR" },
		{ "/",	"OPERATOR" },
		{ "==", "OPERATOR" },
		{ "^=", "OPERATOR" },
		{ "<",	"OPERATOR" },
		{ ">",	"OPERATOR" },
		{ "=<", "OPERATOR" },
		{ "=>", "OPERATOR" }
	});
	seperators.insert({
		//COMMENT
		{ "!",	"SEPERATOR" },
		//SEPERATORs
		{ "%%", "SEPERATOR" },
		{ ":",	"SEPERATOR" },
		{ "[",	"SEPERATOR" },
		{ "]",	"SEPERATOR" },
		{ ",",	"SEPERATOR" },
		{ "{",	"SEPERATOR" },
		{ "}",	"SEPERATOR" },
		{ "(",	"SEPERATOR" },
		{ ")",	"SEPERATOR" },
		{ ";",	"SEPERATOR" }
	});
	keywords.insert({
		//KEYWORDs
		{ "function", "KEYWORD" },
		{ "int",	"KEYWORD" },
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

//Destructor
LexAn::~LexAn() {

}

bool LexAn::isSeperator(string str) {
	unordered_map<string, string>::iterator it;
	bool foundSep = false;

	it = seperators.find(str);
	

	if (it != seperators.end()) {
		foundSep = true;
	}

	return foundSep;
}

bool LexAn::isOperator(string str) {
	unordered_map<string, string>::iterator it;
	bool foundOp = false;

	it = operators.find(str);


	if (it != seperators.end()) {
		foundOp = true;
	}

	return foundOp;
}

bool LexAn::isKeyword(string str) {
	unordered_map<string, string>::iterator it;
	bool foundKey = false;

	it = keywords.find(str);


	if (it != seperators.end()) {
		foundKey = true;
	}

	return foundKey;
}

bool LexAn::isPossibleSepChar(char sep) {
	const int listNum = 11;
	char possibleSepList[listNum] = { '!', '%', ':', '[', ']', ',', '{', '}', '(', ')', ';' };
	for (int i = 0; i < listNum; i++) {
		if (possibleSepList[i] == sep) {
			return true;
		}
	}
	return false;
}

bool LexAn::isPossibleOpChar(char op) {
	const int listNum = 9;
	char possibleOpList[listNum] = { '=', '+', '-', '*', '/', '=', '^', '<', '>' };

	for (int i = 0; i < listNum; i++) {
		if (possibleOpList[i] == op) {
			return true;
		}
	}
	return false;
}

//string LexAn::getLexemeName(int state) {
//	if (state == 2 || state == 5) {
//		return "INTEGER";
//	}
//	else if (state == 6 || state == 3) {
//		return "IDENTIFIER";
//	}
//	else if (state == 8) {
//		return "IDENTIFIER";
//	}
//	else if (state == 10) {
//		return "REAL";
//	}
//	else if (state == 13) {
//		return "OPERATOR";
//	}
//	else
//		return "UNKNOWN";
//}

/****************************************************************************************************
 *	FUNCTION lexer
 * --------------------------------------------------------------------------------------------------
 *	1. gets char from file
 *		- until sees seperator, operator, whitespace, eof
 *		- if is is sep, op, or whitespace -> flag to break loop & set character into string & unget
 *		- otherwise create identifier string
 *	2. Check if string is an Seperator, Operator, integer, Real, keyword, or identifier & SET
 *		 appropriate values for Token and Lexeme
 *		- This can be broken up into a "classification" function that returns a number
 *			- DFSM functions can be called in here
 *		- number can be used in a switch statement to set appropriate lexeme and token private 
 *			members
 *		- need to add special case for seperator "%%" -> needs to file.peek()
 *		- WARNING - may be more special cases
 ****************************************************************************************************/
void LexAn::lexer(ifstream& inFile) {
	string lexStr;		//Used to hold and build lexeme
	string temp;
	unordered_map<string, string>::iterator it;
	bool found = false;
	char ch = ' ';
	char peekCh = ' ';
	LexType  handler;			

	while (!found) { //Loops if nothing found or string (id, key, int, real) is still being created
		ch = inFile.get();

		//Check if character is possible seperator, possible operator, wSpace, or EOF
		if (isPossibleSepChar(ch)) {					//Possibly Found a Seperator
			if (!lexStr.empty()) {
				inFile.unget();							//We have a string but hit a sperator
				handler = STRING;
				found = true;
			}
			else {
				handler = SEPERATOR;
				found = true;
			}
		}
		else if(isPossibleOpChar(ch)) {					//Possibly found an Operator
			if (!lexStr.empty()) {
				inFile.unget();							//We have a string but hit operator
				handler = STRING;
				found = true;
			}
			else {
				handler = OPERATOR;
				found = true;
			}
		}
		else if (isspace(ch) && !lexStr.empty()) {		//WhiteSpace Seperation of string
			found = true;
			handler = STRING;
		}
		else if (ch == -1) {							 //END of FILE CASE
			found = true;
			handler = ENDFILE;
		}
		else if (!isspace(ch)) { 						//String creation - Should only add Char if not whitespace, seperator, operator, eof
			lexStr += ch;
		}
		else {
			cout << "ERROR - Line 239 - character unknown"; // Error - Commment out later
			handler = ERR;
		}//ENDIF
	
	}//ENDWHILE

	switch (handler) {
		case STRING:
			if (DFSM(lexStr)) {			//CALLs DFSM, IF returned TRUE:
				
				it = keywords.find(lexeme);
				if (it != seperators.end()) { //- if is a keyword, change token
					setToken(it->second);
					//setLexeme(it->first);
				}
			}
			else {						//	IF returned FALSE:	
				setToken("UNKNOWN");		//		- then DFSM believes it is an UNKNOWN Lexeme 
				//setLexeme(lexStr);		//		- Set string as lexeme and token as UNKNOWN
			}
			break;
		case OPERATOR:
			//HANDLES OPERATORS
			break;
		case SEPERATOR:
			//Handling Seperators
			temp.clear();
			temp.push_back(ch);				// Adding char from While Loop to lex string
			ch = inFile.peek();				// Checking next character
			// %% CASE
			if (temp[0] == '%' && ch == '%') {
				temp.push_back( inFile.get());		//Adds %% to temp string
			}
			//Check Seperator against Seperator Table
			
				//temp.clear();
				//temp.push_back(ch);
				//it = seperators.find(temp);
				//if (temp == "%") {
				//	ch.
				//}
				//if (it != seperators.end()) {

				//	found = true;
				//}
			break;
		case ENDFILE:
			//Handling EOF - means theres nothing left to process
			break;
		default:
			//Output Unknown/ERROR
			// Error message or Unknown
			;
	}



}

void LexAn::setToken(string str) {
	token = str;
}

void LexAn::setLexeme(string str) {
	lexeme = str;
}

//Accessors
string LexAn::getToken() {
	return token;
}

string LexAn::getLexeme() {
	return lexeme;
}

