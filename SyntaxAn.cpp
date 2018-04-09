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

/*
Dev Note:
- If a rule has | <empty>
	- wouldn't it never thrown an error?
*/

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

void SyntaxAn::reportLexerResults() {
	cout << "Token: " << lex.getToken() << setw(10) << "Lexeme: " << lex.getLexeme << endl;
}

//Jeffrey:
//R1. <Rat18S>  :: = <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>
bool SyntaxAn::Rat18S() {
	cout << "<Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
	if (OptFunctionDefinitions()) {
		lex.lexer(file);
		if (lex.getLexeme() == "%%") {
			reportLexerResults();
			if (OptDeclarationList()) {
				if (StatementList()) {
					return true;
				}
				else {
					reportErr("R1 Violated: Statement List Missing");
					return false;
				}
			}
			else {
				reportErr("R1 Violated: Opt Delaration List Missing");
				return false;
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
	cout << "<Opt Function Definitions> -> <Function Definitions> | <Empty>" << endl;
	if (FunctionDefinitions()) {
		return true;
	}
	else if (empty()) {
		return true;
	}
	else {
		reportErr("R2 Violated");
		return false;
	}
}
//R3. <Function Definitions>  :: = <Function> | <Function> <Function Definitions>
bool  SyntaxAn::FunctionDefinitions() {
	cout << "<Function Definitions> -> <Function> | <Function> <Function Definitions>" << endl;
	if (Function()) {
		if (FunctionDefinitions()) {
			return true;
		}
		return true;
	}
	else {
		reportErr("R3 Violated: Function undefined");
		return false;
	}
}
//R4. <Function> :: = function  <Identifier>[<Opt Parameter List>]  <Opt Declaration List>  <Body>
bool  SyntaxAn::Function() {
	cout << "<Function> -> function  <Identifier>[<Opt Parameter List>]  <Opt Declaration List>  <Body>" << endl;
	lex.lexer(file);
	if (lex.getLexeme() == "function"){
		reportLexerResults();
		lex.lexer(file);
		if (lex.getToken == "IDENTIFIER") {
			reportLexerResults();
			lex.lexer(file);
			if (lex.getLexeme == "[") {
				reportLexerResults();
				if (OptParameterList()) {
					lex.lexer(file);
					cout << "Token: " << lex.getToken() << setw(10) << "Lexeme: " << lex.getLexeme() << endl;
					if (lex.getLexeme == "]") {
						if (OptDeclarationList()) {
							if (Body()) {
								return true;
							}
							else {
								reportErr("R4 Violated: Body Missing");
								return false;
							}
						}
						else {
							reportErr("R4 Violated: OptDeclarationList Missing");
							return false;
						}
					}
					else {
						reportErr("R4 Violated: Expected Lexeme]");
						return false;
					}
				}
				else {
					reportErr("R4 Violated: OptParameterList missing");
					return false;
				}
			}
			else {
				reportErr("R4 Violated: Expected Lexeme: [");
				return false;
			}
		}
		else {
			reportErr("R4 Violated: Expected Token: IDENTIFIER");
			return false;
		}
	}
	else {
		reportErr("R4 Vioated: Expected Lexeme: function");
		return false;
	}
}
//R5. <Opt Parameter List> :: = <Parameter List> | <Empty>
bool  SyntaxAn::OptParameterList() {
	cout << "<Opt Paramter List> -> <Parameter List> | <Empty>" << endl;
	if (ParameterList()) {
		return true;
	}
	else if (empty()) {
		return true;
	}
	else {
		reportErr("R5 Violated: Opt Parameter List Violation");
		return false;
	}
}
//R6. <Parameter List>  :: = <Parameter> | <Parameter>, <Parameter List>
bool  SyntaxAn::ParameterList() {
	cout << "<Paramter List> -> <Parameter> | <Parameter>, <Parameter List>" << endl;
	if (Parameter()) {
		lex.lexer(file);
		if (lex.getLexeme == ",") {
			reportLexerResults();
			if (ParameterList()) {
				return true;
			}
			else {
				reportErr("R6 Violated: Expecting Paramter after a comma");
				return false;
			}
		}
		else {

		}
	}
	else {
		reportErr("R6 Violated: ParamaterList Violation");
		return false;
	}
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
	lex.lexer(file);
	cout << "<Qualifier> -> int | boolean | real" << endl;
	if (lex.getLexeme() == "int" || lex.getLexeme() == "boolean" || lex.getLexeme() == "real") {
		return true;
	}
	return false;
}

bool SyntaxAn::Body() {
	cout << "<Body> -> { <Statement list> }" << endl;
	lex.lexer(file);
	if (lex.getLexeme() == "{") {
		if (StatementList()) {
			lex.lexer(file);
			if (lex.getLexeme() == "}") {
				return true;
			}
			else {
				reportErr("R9 violated: expected lexeme: }");
				return false;
			}
		}
		else {
			reportErr("R9 violated");
		}
	}
	else {
		reportErr("R9 violated: expected lexeme: {");
		return false;
	}
	return false;
}

bool SyntaxAn::OptDeclarationList() {
	cout << "<Opt Declaration List> -> <Declaration List> | <Empty>" << endl;
	if (DeclarationList()) {
		return true;
	}
	else if (empty()) {
		return true;
	}
	else {
		reportErr("R10 violated");
		return false;
	}
	return false;
}

bool SyntaxAn::DeclarationList() {
	cout << " <Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>" << endl;
	if (Declaration()) {
		lex.lexer(file);
		if (lex.getLexeme() == ";") {
			if (DeclarationList()) {
				return true;
			}
			return true;
		}
		else {
			reportErr("R11 violated: expected lexeme: ;");
			return false;
		}
	}
	else {
		reportErr("R11 violated");
		return false;
	}
	return false;
}

bool SyntaxAn::Declaration() {
	cout << "<Declaration> -> <Qualifier > <IDs>" << endl;
	if (Qualifier()) {
		if (IDs()) {
			return true;
		}
		else {
			reportErr("R12 violated");
			return false;
		}
	}
	else {
		reportErr("R12 violated");
		return false;
	}
	return false;
}

bool SyntaxAn::IDs() {
	cout << " <IDs> -> <Identifier> | <Identifier>, <IDs>" << endl;
	lex.lexer(file);
	if (lex.getToken() == "identifier") {
		lex.lexer(file);
		if (lex.getLexeme == ",") {
			return IDs();
		}
		return true;
	}
	else {
		reportErr("R13 violated: expected token: identifier");
		return false;
	}
	return false;
}

bool SyntaxAn::StatementList() {
	cout << " <Statement List> -> <Statement> | <Statement> <Statement List>" << endl;
	if (statement()) {
		if (StatementList()) {
			return true;
		}
		return false;
	}
	else {
		reportErr("R14 violated");
		return false;
	}
	return false;
}

bool SyntaxAn::Relop() {
	cout << "<Relop> -> == | ^= | > | < | => | =<" << endl;
	lex.lexer(file);
	if (lex.getLexeme() == "==" || lex.getLexeme() == "^=" || lex.getLexeme() == ">" || lex.getLexeme() == "<" || lex.getLexeme() == "=>" || lex.getLexeme() == "=<") {
		return true;
	}
	else {
		reportErr("R24 violated: expected lexeme: == | ^= | > | < | => | =< ");
		return false;
	}
	return false;
}

bool SyntaxAn::Condition() {
	cout << "<Condition> -> <Expression>  <Relop>   <Expression>" << endl;
	if (expression()) {
		if (Relop()) {
			if (expression())
				return true;
			else {
				reportErr("R23 violated");
				return false;
			}
		}
		else {
			reportErr("R23 violated");
			return false;
		}
	}
	else {
		reportErr("R23 violated");
		return false;
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
bool SyntaxAn::statement() {
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
	else if (Return()) {
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

bool SyntaxAn::assign() {
	cout << "<Assign> -> <Identifier> = <Expression>" << endl;
	lex.lexer(file); 
	if (lex.getToken() == "identifier") {
		lex.lexer(file); 
		if (lex.getLexeme() == "=") {
			if (expression()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ";") {
					return true;
				}
			}
		}
	}
	return false;
}

bool SyntaxAn::expression() {
	cout << "<Expression> -> <Term> <Expression Prime>" << endl;;
	if (term()) {
		if (expressionPrime()) {
			return true;
		}
	}
	return false;
}

bool SyntaxAn::expressionPrime() {
	lex.lexer(file); 
	if (lex.getLexeme() == "+") {
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
		}
	}
	else if (lex.getLexeme() == "-") {
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

bool SyntaxAn::While() {
	cout << "<Statement> -> <While>" << endl;
	cout << "<While> -> ( <Condition>  )  <Statement> " << endl;
	lex.lexer(file); 
	if (lex.getLexeme() == "while") {
		lex.lexer(file); 
		if (lex.getLexeme == "(") {
			if (Condition()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					if (statement()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool SyntaxAn::If() {
	cout << "<Statement> -> <If>" << endl;
	cout << "<If> -> if  ( <Condition>  ) <Statement>   endif |\n if  ( <Condition>  ) <Statement>   else  <Statement>  endif" << endl;
	lex.lexer(file); 
	if (lex.getLexeme() == "if") {
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			if (Condition()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					if (statement()) {
						lex.lexer(file); 
						if (lex.getLexeme() == "endif") {
							return true;
						}
						else if (lex.getLexeme() == "else") {
							if (statement()) {
								if (lex.getLexeme() == "endif") {
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

bool SyntaxAn::term() {
	cout << "<Term> -> <Factor> <Term Prime>" << endl;
	if (factor()) {
		if (termPrime()) {
			return true;
		}
	}
}

bool SyntaxAn::primary() {
	lex.lexer(file); 
	if (lex.getToken() == "identifier") {
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			if (IDs()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					cout << "<Primary> -> <Identifier> (<IDs>)" << endl;
					return true;
				}
			}
		}
		else {
			cout << "<Primary> -> <Identifier>" << endl;
			return true;
		}
	}
	else if (lex.getToken() == "INTEGER") {
		cout << "<Primary> -> <Integer>" << endl;
		return true;
	}
	else if (lex.getLexeme() == "(") {
		if (expression()) {
			lex.lexer(file); 
			if (lex.getLexeme() == ")") {
				cout << "<Primary> -> (<Expression>)" << endl;
				return true;
			}
		}
	}
	else if (lex.getToken() == "REAL") {
		cout << "<Primary> -> <Real>" << endl;
		return true;
	}
	else if (lex.getLexeme() == "true") {
		cout << "<Primary> -> true" << endl;
		return true;
	}
	else if (lex.getLexeme() == "false") {
		cout << "<Primary> -> false" << endl;
		return true;
	}
	return false;
}

bool SyntaxAn::factor() {
	lex.lexer(file); 
	if (lex.getLexeme() == "-") {
		cout << "<Factor> -> -<Primary>"; 
			
			if (primary()) {
				return true;
			}
	}
	else if (primary()) {
		cout << "<Factor> -> <Primary>"; 
			return true;
	}
	return false;
}

bool SyntaxAn::termPrime() {
	lex.lexer(file); 
	if (lex.getLexeme() == "*") {
		cout << "<Term Prime> -> * <Term> <Term Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
			else {
				reportErr("R28 violated: expected <Expression Prime>");
				return false; 
			}
		}
		else {
			reportErr("R28 violated: expected <term>");
			return false; 
		}
	}
	else if (lex.getLexeme() == "/") {
		cout << "<Term Prime> -> / <Term> <Term Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
			else {
				reportErr("R28 violated: expected <Expression Prime>");
				return false; 
			}
		}
		else {
			reportErr("R28 violated: expected <term>");
			return false; 
		}
	}
	else if (empty()) {
		return true;
	}
	return false;
}

bool SyntaxAn::scan() {
	lex.lexer(file); 
	if (lex.getLexeme() == "get") {
		cout << "<Scan> -> get(<IDs>);" << endl;
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			if (IDs()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					lex.lexer(file); 
					if (lex.getLexeme() == ";") {
						return true;
					}
					else {
						reportErr("R21 violated: expected lexeme: ;");
						return false; 
					}
				}
				else {
					reportErr("R21 violated: expected lexeme: )");
					return false; 
				}
			}
			else {
				reportErr("R21 violated: expected <IDs>");
				return false; 
			}
		}
		else {
			reportErr("R21 violated: expected lexeme: (");
			return false; 
		}
	}
	else {
		reportErr("R21 violated: expected lexeme: get");
	}
	return false;
}

bool SyntaxAn::print() {
	lex.lexer(file); 
	if (lex.getLexeme == "put") {
		cout << "<Scan> -> put(<expression>);" << endl;
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			if (expression()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					lex.lexer(file); 
					if (lex.getLexeme() == ";") {
						return true;
					}
					else {
						reportErr("R20 violated: expected lexeme: ;");
						return false; 
					}
				}
				else {
					reportErr("R20 violated: expected lexeme: )");
					return false; 
				}
			}
			else {
				reportErr("R20 violated: expected <expression>"); 
				return false; 
			}
		}
		else {
			reportErr("R20 violated: expected lexeme: (");
			return false; 
		}
	}
	else {
		reportErr("R20 violated: expected lexeme: put");
		return false; 
	}
	return false;
}

bool SyntaxAn::compound() {
	lex.lexer(file); 
	if (lex.getLexeme() == "{") {
		cout << "<Compound> -> {  <Statement List>  } " << endl;
		if (statement()) {
			lex.lexer(file); 
			if (lex.getLexeme() == "}") {
				return true;
			}
			else {
				reportErr("R16 violated: expected lexeme: }");
				return false; 
			}
		}
		else {
			reportErr("R16 violated: expected <statement>");
			return false; 
		}
	}
	else {
		reportErr("R16 violated: expected lexeme: {");
		return false; 
	}
	return false;
}

bool SyntaxAn::Return() { 
	lex.lexer(file); 
	if (lex.getLexeme == "return") {
		lex.lexer(file); 
		if (lex.getLexeme() == ";") {
			cout << "<Return> -> return;" << endl;
			return true;
		}
		else if (expression()) {
			cout << "<Return> -> return<expression>;" << endl;
			lex.lexer(file); 
			if (lex.getLexeme() == ";") {
				return true;
			}
		}
		else {
			reportErr("R19 violated: expected lexeme: ;"); 
			return false; 
		}
		return false;
	}
	else {
		reportErr("R19 violated: expected lexeme: \'return\'");
		return false; 
	}
}

bool empty() {
	//WHAT DO WE DO HEREREREREREREEEEEEEEEEEEEE

	return true;
}