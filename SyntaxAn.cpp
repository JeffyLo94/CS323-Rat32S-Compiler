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
*		- Implementation of Syntax Analyzer Class
***********************************************************/
#include "SyntaxAn.h"

SyntaxAn::SyntaxAn(string filename) {
	file.open(filename);
	if (!file.is_open()) {
		cerr << "Unable to open source file " << filename;
	}
}

SyntaxAn::~SyntaxAn() {
	file.close();
}

void SyntaxAn::reportErr(string msg) {
	cout << "Error at Line Number: " << lex.getCurrLineNum() << endl;
	cout << "Syntax Error Lexeme: " << lex.getLexeme() << " Token: " << lex.getToken() << endl;
	cout << msg << endl << endl;
}

//Jeffrey:
//R1. <Rat18S>  :: = <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>
bool SyntaxAn::Rat18S() {
	cout << "<Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
	if (OptFunctionDefinitions()) {
		lex.lexer(file);
		if (lex.getLexeme() == "%%") {
			if (OptDeclarationList()) {
				if (StatementList()) {
					return true;
				}
			}
		}
		else {
			reportErr("R1 Violated: Expected Lexeme: %%");
			return false;
		}
	}
	else {
		reportErr("R1 Violated");
		return false;
	}
}

//R2. <Opt Function Definitions> :: = <Function Definitions> | <Empty>
bool SyntaxAn::OptFunctionDefinitions() {
	if (FunctionDefinitions()) {
		return true;
	}
	else if(empty()){
		return true;
	}
	else {
		reportErr("R2 Violated");
		return false;
	}
}
//R3. <Function Definitions>  :: = <Function> | <Function> <Function Definitions>
bool  SyntaxAn::FunctionDefinitions() {

}
//R4. <Function> :: = function  <Identifier>[<Opt Parameter List>]  <Opt Declaration List>  <Body>
bool  SyntaxAn::Functions() {

}
//R5. <Opt Parameter List> :: = <Parameter List> | <Empty>
bool  SyntaxAn::OptParameterList() {

}
//R6. <Parameter List>  :: = <Parameter> | <Parameter>, <Parameter List>
bool  SyntaxAn::ParameterList() {

}
//R7. <Parameter> :: = <IDs > : <Qualifier>
bool  SyntaxAn::Parameter() {

}


//Arman
//R8. <Qualifier> :: = int | boolean | real
//R9. <Body>  :: = { < Statement List> }
//R10. <Opt Declaration List> :: = <Declaration List> | <Empty>
//R11. <Declaration List>  : = <Declaration>; | <Declaration>; <Declaration List>
//R12. <Declaration> :: = <Qualifier > <IDs>
//R13. <IDs> :: = <Identifier> | <Identifier>, <IDs>
//R14. <Statement List> :: = <Statement> | <Statement> <Statement List>
//R24. <Relop> :: = == | ^= | > | < | => | =<
//R23. <Condition> :: = <Expression>  <Relop>   <Expression>
bool SyntaxAn::Qualifier() {
	/*lexar call*/
	if (lexeme == "int" || lexeme == "boolean" || lexeme == "real") {
		return true;
	}
	return false;
}

bool SyntaxAn::Body() {
	/*Lexar call*/
	if (lexeme == "{") {
		if (StatementList()) {
			if (lexeme == "}")
				return true;
		}
	}
	return false;
}

bool SyntaxAn::OptDeclarationList() {
	if (DeclarationList()) {
		return true;
	}
	else if (empty()) {
		return true;
	}
	return false;
}

bool SyntaxAn::DeclarationList() {
	if (Declaration()) {
		if (lexeme == ";") {
			return DeclarationList();
		}
		return true;
	}
	return false;
}

bool SyntaxAn::Declaration() {
	if (Qualifier()) {
		if (IDs()) {
			return true;
		}
	}
	return false;
}

bool SyntaxAn::IDs() {
	if (token == "identifier") {
		if (lexeme == ",") {
			return IDs();
		}
		return true;
	}
	return false;
}

bool SyntaxAn::StatementList() {

	return false;
}

bool SyntaxAn::Relop() {
	if (lexeme == "==" || lexeme == "^=" || lexeme == ">" || lexeme == "<" || lexeme == "=>" || lexeme == "=<") {
		return true;
	}
	return false;
}

bool SyntaxAn::Condition() {
	if (expression()) {
		if (Relop()) {
			if (expression())
				return true;
		}
	}
	return false;
}

//Andrew
//R15. <Statement> :: = <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
//R16. <Compound> :: = { <Statement List> }
//R17. <Assign> :: = <Identifier> = <Expression>;
//R18. <If> :: = if (<Condition>) <Statement>   endif | if (<Condition>) <Statement>   else  <Statement>  endif
//R19. <Return> :: = return; | return <Expression>;
//R20. <Print> :: = put(<Expression>);
//R21. <Scan> :: = get(<IDs>);
//R22. <While> :: = while (<Condition>)  <Statement>
//R25. <Expression> :: = <Term> <Expression` >
//R26. <Expression Prime > :: = +<Term> <Expression Prime> | -<Expression Prime> <Term> | <Empty>
//R27. <Term> :: = <Factor> <Term Prime>
//R28. <Term Prime> :: = *<Factor> <Term Prime> | / <Factor> <Term Prime> | <Empty>
//R29. <Factor> :: = -<Primary> | <Primary>
//R30. <Primary> :: = <Identifier> | <Integer> | <Identifier>  (<IDs>) | (<Expression>) | <Real> | true | false
//R31. <Empty>   :: = e
bool statement() {
	if (compound()) {
		return true;
	}
	else if (assign()) {
		return true;
	}
	else if (While()) {
		return true;
	}
	else if (If()) {
		return true;
	}
	else if (returnFunction()) {
		return true;
	}
	else if (print()) {
		return true;
	}
	else if (scan()) {
		return true;
	}
	return false;
}

bool assign() {
	cout << "<Assign> -> <Identifier> = <Expression>" << endl;
	if (token == "identifier") {
		if (/*Increment a token. However we do it*/ lexeme == "=") {
			if (expression()) {
				if (lexeme == ";") {
					return true;
				}
			}
		}
	}
	return false;
}

bool expression() {
	cout << "<Expression> -> <Term> <Expression Prime>" << endl;;
	if (term()) {
		if (expressionPrime()) {
			return true;
		}
	}
	return false;
}

bool expressionPrime() {
	if (token == "+") {
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
		}
	}
	else if (token == "-") {
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
		}
	}
	else if (empty()) {
		return true;
	}
	return false;
}

bool While() {
	cout << "<Statement> -> <While>" << endl;
	cout << "<While> -> ( <Condition>  )  <Statement> " << endl;
	if (token == "while") {
		if (token == "(") {
			if (condition()) {
				if (token == ")") {
					if (statement()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool If() {
	cout << "<Statement> -> <If>" << endl;
	cout << "<If> -> if  ( <Condition>  ) <Statement>   endif |\n if  ( <Condition>  ) <Statement>   else  <Statement>  endif" << endl;
	if (token == "if") {
		if (token == "(") {
			if (condition()) {
				if (token == ")") {
					if (statement()) {
						if (token == "endif") {
							return true;
						}
						else if (token == "else") {
							if (statement()) {
								if (token == "endif") {
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool term() {
	cout << "<Term> -> <Factor> <Term Prime>" << endl;
	if (factor()) {
		if (termPrime()) {
			return true;
		}
	}
}

bool primary() {
	if (token == "identifier") {
		if (lexeme == "(") {
			if (IDs()) {
				if (lexeme == ")") {
					cout << "<Primary> -> <Identifier> (<IDs>)" << endl;
					returne true;
				}
			}
		}
		else {
			cout << "<Primary> -> <Identifier>" << endl;
			return true;
		}
	}
	else if (token == "integer") {
		cout << "<Primary> -> <Integer>" << endl;
		return true;
	}
	else if (lexeme == "(") {
		if (expression()) {
			if (token == ")") {
				cout << "<Primary> -> (<Expression>)" << endl;
				return true;
			}
		}
	}
	else if (token == "real") {
		cout << "<Primary> -> <Real>" << endl;
		return true;
	}
	else if (lexeme == "true") {
		cout << "<Primary> -> true" << endl;
		return true;
	}
	else if (lexeme == "false") {
		cout << "<Primary> -> false" << endl;
		return true;
	}
	return false;
}

bool factor() {
	if (lexeme == "-") {
		cout << "<Factor> -> -<Primary>"
			//Get next token
			if (primary()) {
				return true;
			}
	}
	else if (primary()) {
		cout << "<Factor> -> <Primary>"
			return true;
	}
	return false;
}

bool termPrime() {
	if (token == "*") {
		cout << "<Term Prime> -> * <Term> <Term Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
		}
	}
	else if (token == "/") {
		cout << "<Term Prime> -> / <Term> <Term Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
		}
	}
	else if (empty()) {
		return true;
	}
	return false;
}

bool scan() {
	if (lexeme == "get") {
		cout << "<Scan> -> get(<IDs>);" << endl;
		if (lexeme == "(") {
			if (IDs()) {
				if (lexeme == ")") {
					if (lexeme == ";") {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool print() {
	if (lexeme == "put") {
		cout << "<Scan> -> put(<expression>);" << endl;
		if (lexeme == "(") {
			if (expression()) {
				if (lexeme == ")") {
					if (lexeme == ";") {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool compound() {
	if (lexeme == "{") {
		cout << "<Compound> -> {  <Statement List>  } " << endl;
		if (statement()) {
			if (lexeme == "}") {
				return true;
			}
		}
	}
	return false;
}

bool Return() {
	if (lexeme == "return") {
		//get next token
		if (lexeme == ";") {
			cout << "<Return> -> return;" << endl;
			return true;
		}
		else if (expression()) {
			cout << "<Return> -> return<expression>;" << endl;
			if (lexeme == ";") {
				return true;
			}
		}
		return false;
	}
}

bool empty() {
	//WHAT DO WE DO HEREREREREREREEEEEEEEEEEEEE

	return true;
}