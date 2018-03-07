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

//DFSM for Integers, Reals, Identifiers
// Returns True if Accepted, False if not
bool LexAn::DFSM(string str) {
	bool acceptance = false;
	int transistionTable[14][7] =
	{
		{ 0,	letter,	digit,	decimal,	dollar,	space,	punct },
		{ 1,	3,		2,		11,			11,		12,		13 },	//Starting State
		{ 2,	11,		5,		4,			11,		12,		13 },	//Accept Integer
		{ 3,	8,		7,		11,			6,		12,		13 },	//Accepting Identifier
		{ 4,	11,		9,		11,			11,		12,		13 },
		{ 5,	11,		5,		4,			11,		12,		13 },	//Accept Integer
		{ 6,	11,		11,		11,			11,		12,		13 },	//Accept Identifier
		{ 7,	8,		7,		11,			6,		12,		13 },
		{ 8,	8,		7,		11,			6,		12,		13 },	//Accept Identifier/KEYWORD - Check for KEYWORD if last state
		{ 9,	11,		10,		11,			11,		12,		13 },	//Accept Real
		{ 10,	11,		10,		11,			11,		12,		13 },	//Accept Real
		{ 11,	11,		11,		11,			11,		12,		11 },
		{ 12,	12,		12,		12,			12,		12,		12 },
		{ 13,	3,		2,		11,			11,		12,		13 }
	};

	int startingState = 1;
	int strLength = str.length();
	int curState = 1;
	int curCol = 1;
	char currentChar = ' ';

	for (int i = 0; i < strLength; i++) {
		currentChar = str[i];
		curCol = colNum(currentChar);
		curState = transistionTable[curState][curCol];
	}

	if (curState < reject) { //ACCEPTED
		if (curState == 2 || curState == 5) {
			setToken("INTEGER");
			setLexeme(str);
		}
		else if (curState == 9 || curState == 10) {
			setToken("REAL");
			setLexeme(str);
		}
		else if (curState == 3 || curState == 6 || curState == 8) {
			setToken("IDENTIFIER");
			setLexeme(str);
		}
		return acceptance = true;
	}
	else {
		setToken("ERROR-DFSM");
		setLexeme(str);
	}

	

	return acceptance;
}

int LexAn::colNum(char ch) {
	if (isdigit(ch)) {
		return digit;
	}
	else if (isalpha(ch)) {
		return letter;
	}
	else if (ch == '$') {
		return dollar;
	}
	else if (ch == '.') {
		return decimal;
	}
	else if (isspace(ch)) {
		return space;
	}
	else if (ispunct(ch)) {
		return punct;
	}
}

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
	LexType  handler = ERR;			

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
			//cerr << "EOF reached" << endl;
			//if (!lexStr.empty()) {
			//	cerr << "attempt to put back EOF char" << endl;
			//	inFile.putback(ch);
			//	handler = STRING;
			//}
			//else {
			//	cerr << "lexstr is empty" << endl;
			//	handler = ENDFILE;
			//}
			found = true;
		}
		else if (!isspace(ch) && ch != -1) { 						//String creation - Should only add Char if not whitespace, seperator, operator, eof
			lexStr += ch;
		}
		else {
			//Must be white space
			found = false;
			//cout << "ERROR - Line 239 - character unknown"; // Error - Commment out later
			//handler = ERR;
		}//ENDIF
	
	}//ENDWHILE

	//CASE - EOF WhiteSpace
	if (lexStr.empty() && ch == -1) {
		handler = ENDFILE;
	}

	switch (handler) {
		case STRING:
			//cerr << "entering DFSM" << endl;
			if (DFSM(lexStr)) {			//CALLs DFSM, IF returned TRUE:
				it = keywords.begin();
				it = keywords.find(lexeme);
				if (it != keywords.end()) { //- if is a keyword, change token
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
			temp.clear();
			temp.push_back(ch);
			ch = inFile.peek();
			if (temp[0] == '=' && ch == '=' ||				// == CASE
				temp[0] == '=' && ch == '>' ||				// => CASE
				temp[0] == '=' && ch == '<' ||				// =< CASE
				temp[0] == '^' && ch == '='	) {				// ^= CASE
				temp.push_back( inFile.get() );				// Adds second char to string
			}
			lexStr = temp;
			//Check Against Operators list
			it = operators.begin();
			it = operators.find(lexStr);
			if (it != operators.end()) {
				//operator found
				setToken( it->second );
				setLexeme(lexStr);
			}
			else {
				//UNKNOWN Symbol
				setToken("UNKNOWN");
				setLexeme(lexStr);
			}

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

			lexStr = temp;
			//Check Seperator against Seperator Table
			it = seperators.begin();
			it = seperators.find(lexStr);
			if (it != seperators.end()) {
				//found seperator
				setToken(it->second);
				setLexeme(lexStr);
			}
			else {						//UNKNOWN SYMBOL
				setToken("UNKNOWN");
				setLexeme(lexStr);
			}
			break;
		case ENDFILE:
			//Handling EOF - means theres nothing left to process
			setLexeme("EOF");
			setToken("EOF");
			break;
		default:
			//Output Unknown/ERROR
			// Error message or Unknown
			setToken("UNKNOWN");
			setLexeme(lexStr);
			break;
	}// END Switch
}

//Settors
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

void LexAn::print() {
	cout << left << setw(20) << getToken() << setw(20) << getLexeme() << endl;
 }