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

SyntaxAn::SyntaxAn(string inputFile, string outputFile) {
	//Prepping InputFile
	file.open(inputFile);
	if (!file.is_open()) {
		cerr << "InputFile already in use" << outputFile;
	}

	//Prepping OutputFile
	outFile.open(outputFile, std::ofstream::out | std::ofstream::app);
	if (!outFile.is_open()) {
		cerr << "OutFile already in use" << outputFile;
	}

}

SyntaxAn::~SyntaxAn() {
	file.close();
	outFile.close();
}

void SyntaxAn::reportErr(string msg) {
	cout << "Error at Line Number: " << lex.getCurrLineNum() << endl;
	cout << "Syntax Error Lexeme: " << lex.getLexeme() << " Token: " << lex.getToken() << endl;
	cout << msg << endl << endl;
}

void SyntaxAn::reportLexerResults() {
	cout << "Token: " << lex.getToken() << setw(10) << "Lexeme: " << lex.getLexeme() << endl;
}

//Jeffrey:
//R1. <Rat18S>  :: = <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>
bool SyntaxAn::Rat18S() {
	cout << "<Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;

	lex.lexer(file);
	if (lex.getLexeme() == "%%") {
		reportLexerResults();
		lex.lexer(file);
		if (OptDeclarationList()) {
			if (StatementList()) {
				return true;
			}
		}
	}
	else if (OptFunctionDefinitions()) {
		if (lex.getLexeme() == "%%") {
			reportLexerResults();
			lex.lexer(file);
			if (OptDeclarationList()) {
				if (StatementList()) {
					return true;
				}
			}
		}
	}
	if (lex.getLexeme() == "EOF") {
		cout << "Successful parsing" << endl;
		return true;
	}
	else {
		reportErr("R1 Violated: Incomplete RAT18S");
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
		return false;
	}
}
//R4. <Function> :: = function  <Identifier>[<Opt Parameter List>]  <Opt Declaration List>  <Body>
bool  SyntaxAn::Function() {
	cout << "<Function> -> function  <Identifier>[<Opt Parameter List>]  <Opt Declaration List>  <Body>" << endl;
	if (lex.getLexeme() == "function"){
		lex.lexer(file);
		reportLexerResults();
		if (lex.getToken() == "IDENTIFIER") {
			lex.lexer(file);
			reportLexerResults();
			if (lex.getLexeme() == "[") {
				lex.lexer(file);
				if (OptParameterList()) {
					if (lex.getLexeme() == "]") {
						reportLexerResults();
						lex.lexer(file);
						if (lex.getLexeme() == "{") {
							if (Body()) {
								return true;
							}
							else {
								reportErr("R4 Violated: Body Missing");
								return false;
							}
						}
						else if (OptDeclarationList()) {
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
						reportErr("R4 Violated: Expected Lexeme: ]");
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
	return false;
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
}
//R6. <Parameter List>  :: = <Parameter> | <Parameter>, <Parameter List>
bool  SyntaxAn::ParameterList() {
	cout << "<Paramter List> -> <Parameter> | <Parameter>, <Parameter List>" << endl;
	if (Parameter()) {
		if (lex.getLexeme() == ",") {
			reportLexerResults();
			lex.lexer(file);
			if (ParameterList()) {
				return true;
			}
			else {
				reportErr("R6 Violated: Expecting Paramter after a comma");
				return false;
			}
		}
		return true;
	}
	else {
		reportErr("R6 Violated: ParamaterList Violation");
		return false;
	}
}
//R7. <Parameter> :: = <IDs > : <Qualifier>
bool  SyntaxAn::Parameter() {
	cout << "<Paramter> -> <IDs> : <Qualifier>" << endl;
	if (IDs()) {
		if (lex.getLexeme() == ":") {
			reportLexerResults();
			lex.lexer(file);
			if (Qualifier()) {
				return true;
			}
			else {
				reportErr("R7 Violated: Qualifier Missing");
				return false;
			}
		}
		else {
			reportErr("R7 Violated: Expected Lexeme \':\'");
			return false;
		}
	}
	else {
		reportErr("R7 Violated: No IDs");
		return false;
	}
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
	cout << "<Qualifier> -> int | boolean | real" << endl;
	if (lex.getLexeme() == "int" || lex.getLexeme() == "boolean" || lex.getLexeme() == "real") {
		reportLexerResults();
		lex.lexer(file);
		return true;
	}
	return false;
}

bool SyntaxAn::Body() {
	cout << "<Body> -> { <Statement list> }" << endl;
	if (lex.getLexeme() == "{") {
		reportLexerResults();
		lex.lexer(file);
		if (StatementList()) {
			//lex.lexer(file);
			if (lex.getLexeme() == "}") {
				reportLexerResults();
				lex.lexer(file);
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
	//else {
	//	reportErr("R10 violated");
	//	return false;
	//}
	return false;
}

bool SyntaxAn::DeclarationList() {
	cout << " <Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>" << endl;
	if (Declaration()) {
		if (lex.getLexeme() == ";") {
			reportLexerResults();
			lex.lexer(file);
			return true;
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
			reportErr("R12 violated: IDs Missing");
			return false;
		}
	}
	else {
		reportErr("R12 violated: Qualifier Error");
		return false;
	}
	return false;
}

bool SyntaxAn::IDs() {
	cout << " <IDs> -> <Identifier> | <Identifier>, <IDs>" << endl;
	if (lex.getToken() == "IDENTIFIER") {
		reportLexerResults();
		lex.lexer(file);
		if(lex.getLexeme() == ",") {
			reportLexerResults();
			lex.lexer(file);
			return IDs();
		}
		return true;
	}
	else {
		reportErr("R13 violated: expected token: IDENTIFIER");
		return false;
	}
	return false;
}

bool SyntaxAn::StatementList() {
	if (statement() && lex.getLexeme() != "EOF") {
		cout << "<Statement List> -> <Statement> | <Statement> <Statement List>" << endl;
		if (StatementList()) {
			return true;
		}
		return true;
	}
	else {
		if (lex.getLexeme() == "EOF") {
			return false;
		}
		return false;
	}
	return false;
}

bool SyntaxAn::Relop() {
	cout << "<Relop> -> == | ^= | > | < | => | =<" << endl;
	if (lex.getLexeme() == "==" || lex.getLexeme() == "^=" || lex.getLexeme() == ">" || lex.getLexeme() == "<" || lex.getLexeme() == "=>" || lex.getLexeme() == "=<") {
		reportLexerResults();
		lex.lexer(file);
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
	//reportErr("R15 violated expected <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
	return false;
}

bool SyntaxAn::assign() {
	if (lex.getToken() == "IDENTIFIER") {
		cout << "<Assign> -> <Identifier> = <Expression>" << endl;
		reportLexerResults();
		lex.lexer(file); 
		if (lex.getLexeme() == "=") {
			reportLexerResults();
			lex.lexer(file);
			if (expression()) {
				if (lex.getLexeme() == ";") {
					reportLexerResults();
					lex.lexer(file);
					return true;
				}
				else {
					reportErr("R17 violated: expected lexeme: ;"); 
					return false; 
				}
			}
			else {
				reportErr("R17 violated: expected <expression>"); 
				return false; 
			}
		}
		else {
			reportErr("R17 violated: expected lexeme: =");
			return false;
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
		else {
			reportErr("R25 vioalted: expected Expression Prime"); 
			return false; 
		}
	}
	return false;
}

bool SyntaxAn::expressionPrime() {
	if (lex.getLexeme() == "+") {
		reportLexerResults();
		lex.lexer(file); 
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
			else {
				reportErr("R26 violated: expected <expression prime>");
				return false;
			}
		}
		else {
			reportErr("R26 violated: expected <term>");
			return false;
		}
	}
	else if (lex.getLexeme() == "-") {
		reportLexerResults();
		lex.lexer(file); 
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		if (term()) {
			if (expressionPrime()) {
				return true;
			}
			else {
				reportErr("R26 violated: expected <ExpressionPrime>");
				return false;
			}
		}
		else {
			reportErr("R26 violated: expected <term>");
			return false;
		}
	}
	else if (empty()) {
		return true;
	}
	return false;
}

bool SyntaxAn::While() {
	if (lex.getLexeme() == "while") {
		cout << "<Statement> -> <While>" << endl;
		cout << "<While> -> ( <Condition>  )  <Statement> " << endl;
		reportLexerResults();
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			lex.lexer(file);
			if (Condition()) {
				if (lex.getLexeme() == ")") {
					reportLexerResults();
					lex.lexer(file);
					if (statement()) {
						return true;
					}
					else {
						reportErr("R22 violated: expected <statement>");
						return false;
					}
				}
				else {
					reportErr("R22 violated: expected lexeme: )");
					return false; 
				}
			}
			else {
				reportErr("R22 violated: expected <condition>");
				return false;
			}
		}
		else {
			reportErr("R22 violated: expected lexeme (");
			return false;
		}
	}
	return false;
}

bool SyntaxAn::If() {
	if (lex.getLexeme() == "if") {
		cout << "<Statement> -> <If>" << endl;
		cout << "<If> -> if  ( <Condition>  ) <Statement>   endif |\n if  ( <Condition>  ) <Statement>   else  <Statement>  endif" << endl;
		reportLexerResults();
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			reportLexerResults();
			lex.lexer(file);
			if (Condition()) {
				if (lex.getLexeme() == ")") {
					reportLexerResults();
					lex.lexer(file);
					if (statement()) {
						lex.lexer(file); 
						if (lex.getLexeme() == "endif") {
							reportLexerResults();
							lex.lexer(file);
							return true;
						}
						else if (lex.getLexeme() == "else") {
							reportLexerResults();
							lex.lexer(file);
							if (statement()) {
								lex.lexer(file);
								if (lex.getLexeme() == "endif") {
									reportLexerResults();
									lex.lexer(file);
									return true;
								}
								else {
									reportErr("R18 violated: expected lexeme: endif");
									return false;
								}
							}
							else {
								reportErr("R18 violated: expected <statement>");
								return false;
							}
						}
						else {
							reportErr("R18 violated: expected lexeme: else | endif");
							return false;
						}
					}
					else {
						reportErr("R18 violated: expected <statement>");
						return false;
					}
				}
				else {
					reportErr("R18 violated: expected lexeme: )");
					return false;
				}
			}
			else {
				reportErr("R18 violated: expected <condition>");
				return false;
			}
		}
		else {
			reportErr("R18 violated: expected lexeme: ( ");
			return false;
		}
	}
	return false;
}

bool SyntaxAn::term() {
	cout << "<Term> -> <Factor> <Term Prime>" << endl;
	if (factor()) {
		lex.lexer(file);
		if (termPrime()) {
			return true;
		}
		else {
			reportErr("R27 violated: expected <termPrime>"); 
			return false; 
		}
	}
	else {
		reportErr("R27 violated: expected <Term>"); 
		return false; 
	}
}

bool SyntaxAn::primary() {
	char ch = file.peek();
	if (ch == '(') {
		reportLexerResults();
		lex.lexer(file);
		reportLexerResults();
		lex.lexer(file);
		if (IDs()) {
			reportLexerResults();
			return true;
		}
	}
	if (lex.getToken() == "IDENTIFIER") {
		reportLexerResults();
		char ch = file.peek();
		if (ch== '(') {
			lex.lexer(file);
			reportLexerResults();
			if (IDs()) {
				lex.lexer(file); 
				if (lex.getLexeme() == ")") {
					reportLexerResults();
					cout << "<Primary> -> <Identifier> (<IDs>)" << endl;
					return true;
				}
				else {
					reportErr("R30 violated: expected lexeme: (");
					return false; 
				}
			}
			else {
				reportErr("R30 violated: expected <IDs>"); 
				return false; 
			}
		}
		else {
			cout << "<Primary> -> <Identifier>" << endl;
			return true;
		}
	}
	else if (lex.getToken() == "INTEGER") {
		reportLexerResults();
		cout << "<Primary> -> <Integer>" << endl;
		return true;
	}
	else if (lex.getLexeme() == "(") {
		reportLexerResults();
		lex.lexer(file);
		if (expression()) {
			if (lex.getLexeme() == ")") {
				reportLexerResults();
				cout << "<Primary> -> (<Expression>)" << endl;
				return true;
			}
			else {
				reportErr("R30 violated: expected lexeme: )"); 
				return false; 
			}
		}
		else {
			reportErr("R30 violated: expected <expression>"); 
			return false; 
		}
	}
	else if (lex.getToken() == "REAL") {
		reportLexerResults();
		cout << "<Primary> -> <Real>" << endl;
		return true;
	}
	else if (lex.getLexeme() == "true") {
		reportLexerResults();
		cout << "<Primary> -> true" << endl;
		return true;
	}
	else if (lex.getLexeme() == "false") {
		reportLexerResults();
		cout << "<Primary> -> false" << endl;
		return true;
	}
	else {
		reportErr("R30 violated"); 
		return true; 
	}
	return false;
}

bool SyntaxAn::factor() {
	if (lex.getLexeme() == "-") {
		reportLexerResults();
		cout << "<Factor> -> -<Primary>" << endl; 
			if (primary()) {
				return true;
			}
	}
	else if (primary()) {
		cout << "<Factor> -> <Primary>" << endl;
		return true;
	}
	else {
		reportErr("R29 violated"); 
		return false; 
	}
	return false;
}

bool SyntaxAn::termPrime() {
	if (lex.getLexeme() == "*") {
		reportLexerResults();
		lex.lexer(file);
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
		reportLexerResults();
		lex.lexer(file); 
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
	if (lex.getLexeme() == "get") {
		cout << "<Scan> -> get(<IDs>);" << endl;
		reportLexerResults();
		lex.lexer(file); 
		if (lex.getLexeme() == "(") {
			reportLexerResults();
			lex.lexer(file);
			if (IDs()) {
				if (lex.getLexeme() == ")") {
					reportLexerResults();
					lex.lexer(file); 
					if (lex.getLexeme() == ";") {
						reportLexerResults();
						lex.lexer(file);
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
	return false;
}

bool SyntaxAn::print() {
	if (lex.getLexeme() == "put") {
		cout << "<Scan> -> put(<expression>);" << endl;
		reportLexerResults();
		lex.lexer(file);
		if (lex.getLexeme() == "(") {
			reportLexerResults();
			lex.lexer(file);
			if (expression()) {
				reportLexerResults();
				if (lex.getLexeme() == ")") {
					lex.lexer(file); 
					if (lex.getLexeme() == ";") {
						reportLexerResults();
						lex.lexer(file);
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
	return false;
}

bool SyntaxAn::compound() {
	if (lex.getLexeme() == "{") {
		reportLexerResults();
		lex.lexer(file);
		cout << "<Compound> -> {  <Statement List>  } " << endl;
		if (StatementList()) {
			if (lex.getLexeme() == "}") {
				reportLexerResults();
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
	return false; 
}

bool SyntaxAn::Return() { 
	if (lex.getLexeme() == "return") {
		reportLexerResults();
		lex.lexer(file); 
		if (lex.getLexeme() == ";") {
			reportLexerResults();
			lex.lexer(file);
			cout << "<Return> -> return;" << endl;
			return true;
		}
		else if (expression()) {
			cout << "<Return> -> return<expression>;" << endl;
			if (lex.getLexeme() == ";") {
				reportLexerResults();
				lex.lexer(file);
				return true;
			}
		}
		else {
			reportErr("R19 violated: expected lexeme: ;"); 
			return false; 
		}
		return false;
	}
	return false;
}

bool SyntaxAn::empty() {
	return true;
}