#include <iostream>
#include <string>
using namespace std;
/*
R8. <Qualifier> ::= int     |    boolean    |  real
R9. <Body>  ::=  {  < Statement List>  }
R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>
R12. <Declaration> ::=   <Qualifier > <IDs>
R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>
R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>
R24. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<
R23. <Condition> ::=     <Expression>  <Relop>   <Expression>
*/

string token = "blah";
string lexeme;

int main() {

  return 0;
}

bool Qualifier() {
  /*lexar call*/
  if(lexeme == "int" || lexeme == "boolean" || lexeme == "real") {
    return true;
  }
  return false;
}

bool Body() {
  /*Lexar call*/
  if(lexeme == "{"){
    if(StatementList()) {
      if(lexeme == "}")
        return true;
    }
  }
  return false;
}

bool OptDeclarationList() {
  if(DeclarationList()) {
    return true;
  }
  else if(Empty()) {
    return true; 
  }
  return false;
}

bool DeclarationList() {
  if(Declaration()) {
    if(lexeme == ";") {
      return DeclarationList();
    }
    return true;
  }
  return false;
}

bool Declaration() {
  if(Qualifier()){
      if(IDs()){
        return true;
      }
  }
  return false;
}

bool IDs() {
  if(token == "identifier") {
    if(lexeme == ",") {
      return IDs();
    }
    return true;
  }
  return false;
}

bool StatementList() {

  return false;
}

bool Relop() {
  if( lexeme == "==" || lexeme == "^=" || lexeme ==">" || lexeme == "<" ||  lexeme == "=>" || lexeme =="=<") {
    return true;
  }
  return false;
}

bool Condition() {
  if(Expression()) {
    if(Relop()) {
      if(Expression())
        return true;
    }
  }
  return false;
}
