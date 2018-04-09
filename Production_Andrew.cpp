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
	if (lexeme == "-" ) {
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