%{
#include <iostream>
#include <vector>
#include <list>

#include "Statement.h"
#include "Assignment.h"
#include "PrintStatement.h"
#include "SymbolTable.h"
#include "Value.h"
#include "AST.h"

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();
void yyerror(const char * s);

using namespace std;

%}

%union {
     char*       string;
     int         integer;
     float       real;
     bool        boolean;
     Value*      value;
     AST*        node;
     list<AST*>* astList;
     list<Statement*>* stmtList;
     Statement*        stmt;
     list<Parameter*>* paramList;
     Parameter*        param;
}

%start progr
%token VARIABLE_DEF CONST OBJECT_DEF FUNCTION_DEF FOR IF ELSE THEN WHILE PRINT RETURN TYPE_OF EVAL
%token ASSIGN PLUS MINUS MULT DIV EQUAL LESS GREATER LESSEQUAL GREATEREQUAL NOTEQUAL
%token COLON SEMICOLON COMMA DOT DOT2 CALL
%token OPENPAREN CLOSEPAREN OPENBRACKET CLOSEBRACKET OPENSQUARE CLOSESQUARE
%token<string>    ID TYPE
%token<real>      FLOAT_LITERAL
%token<integer>   INT_LITERAL 
%token<string>    STRING_LITERAL CHAR_LITERAL
%token<boolean>   TRUE FALSE 

%nonassoc THEN
%nonassoc ELSE
%left DOT PLUS MINUS MULT DIV OPENPAREN CLOSEPAREN

// non-terminal symbols 
%type <node>      literal factor term expr extended_expr function_call
%type <astList>   argument_list
%type <stmtList>  code_block statements
%type <stmt>      statement print assignment if_statement for_statement while_statement 
%type <stmt>      return_statement function_stmt eval type_of
%type <param>     parameter
%type <paramList> parameters

%%

progr: user_data_types global_variables function_definitions {printf("The program is correct!\n");}
     ;

parameter :  ID COLON TYPE { $$ = new Parameter(yylineno, $1, $3); }
          ;

parameters : parameter                  { $$ = new list<Parameter*>(); $$->push_back($1); }
           | parameters COMMA parameter { $1-> push_back($3); $$ = $1; }
           |                            { $$ = new list<Parameter*>(); }
           ;

members : global_variables function_definitions
        ;

user_data_types : user_data_type user_data_types 
                |
                ;

user_data_type : OBJECT_DEF ID OPENBRACKET members CLOSEBRACKET
               ;

literal   : FLOAT_LITERAL  { $$ = new AST(yylineno, new FloatValue($1)); }
          | INT_LITERAL    { $$ = new AST(yylineno, new IntValue($1)); }
          | STRING_LITERAL { $$ = new AST(yylineno, new StringValue($1)); }
          | CHAR_LITERAL   { $$ = new AST(yylineno, new CharValue($1[0])); }
          | TRUE           { $$ = new AST(yylineno, new BoolValue($1)); }
          | FALSE          { $$ = new AST(yylineno, new BoolValue($1)); }
          ;

global_variable : VARIABLE_DEF ID COLON TYPE SEMICOLON
                         { SymbolTable::getInstance()->addVariable($4, $2, yylineno);  }
                | VARIABLE_DEF ID COLON ID OPENPAREN argument_list CLOSEPAREN SEMICOLON
                         {
                              cout << "Define userdef variable " << $2 << " of type " << $4 << endl; 
                              SymbolTable::getInstance()->addVariable($4, $2, yylineno);
                         }
                | VARIABLE_DEF ID COLON TYPE OPENPAREN literal CLOSEPAREN SEMICOLON
                         { Variable* v = SymbolTable::getInstance()->addVariable($4, $2, yylineno);
                           v->setValue($6->getValue()); }
                | VARIABLE_DEF ID COLON TYPE OPENSQUARE INT_LITERAL CLOSESQUARE SEMICOLON
                         { cout << "Define array variable " << $2 << " of type " << $4 << endl;}
                | CONST ID COLON TYPE OPENPAREN literal CLOSEPAREN SEMICOLON
                         { Variable* v = SymbolTable::getInstance()->addVariable($4, $2, yylineno);
                           v->setValue($6->getValue());
                           v->setConstant(); }
                ;

global_variables : global_variables global_variable
                 |
                 ;

factor: literal
     | ID OPENSQUARE extended_expr CLOSESQUARE    // access array element
               {  $$ = new AST(yylineno, $1, $3); }
     | ID DOT ID OPENPAREN argument_list CLOSEPAREN // accessing object method
               {
                    //printf("%d: Accessing object method {%s.%s(AST, AST ...)}.\n", yylineno, $1, $3);
                    $$ = new AST(yylineno, $1, $3, $5);
               }
     | ID DOT ID OPENSQUARE extended_expr CLOSESQUARE     // access array element member of an objet
               { printf("%d: Accessing object array field {%s.%s[AST]}.\n", yylineno, $1, $3); }
     | ID DOT ID                                       
               { 
                    //printf("%d Accessing object filed {%s.%s}.\n", yylineno, $1, $3);
                    $$ = new AST(yylineno, $1, $3);
               }
     | ID      {  $$ = new AST(yylineno, $1); } // accesing simple variable/parameter
     | function_call
     | OPENPAREN extended_expr CLOSEPAREN
               { $$ = $2; }
     ;

term : factor 
     | factor MULT factor { $$ = new AST(yylineno, $1, $3, '*');}
     | factor DIV factor  { $$ = new AST(yylineno, $1, $3, '/');}
     ;

expr : term 
     | term PLUS term     { $$ = new AST(yylineno, $1, $3, '+');}
     | term MINUS term    { $$ = new AST(yylineno, $1, $3, '-');}
     ; 

extended_expr : expr 
              | expr EQUAL expr          { $$ = new AST(yylineno, $1, $3, '='); }
              | expr LESS expr           { $$ = new AST(yylineno, $1, $3, '<'); }
              | expr GREATER expr        { $$ = new AST(yylineno, $1, $3, '>'); }
              | expr LESSEQUAL expr      { $$ = new AST(yylineno, $1, $3, AST_LEQ); }
              | expr GREATEREQUAL expr   { $$ = new AST(yylineno, $1, $3, AST_GEQ); }
              | expr NOTEQUAL expr       { $$ = new AST(yylineno, $1, $3, AST_NOT_EQ); }
              ;

assignment : ID ASSIGN extended_expr
               { 
                    // cout << yylineno << " ASSIGN " << $1 << " <- " << "$3" << endl;
                    $$ = new Assignment(yylineno, $1, $3);
               }
           ;

if_statement : IF OPENPAREN extended_expr CLOSEPAREN THEN statement
                    {
                         //cout << yylineno << ": IF <expr> THEN <stmt>"  << endl; 
                         $$ = new IfStatement(yylineno, $3, $6, nullptr);
                    }
             | IF OPENPAREN extended_expr CLOSEPAREN THEN statement ELSE statement
                    {    
                         //cout << yylineno << ": IF <expr> THEN <stmt> ELSE <stmt>"  << endl; 
                         $$ = new IfStatement(yylineno, $3, $6, $8);
                    }
             ;

for_statement : FOR OPENPAREN global_variable SEMICOLON extended_expr SEMICOLON extended_expr CLOSEPAREN statement
               { $$ = new EmptyStatement(yylineno); }
              ;

while_statement : WHILE OPENPAREN extended_expr CLOSEPAREN statement
               { $$ = new EmptyStatement(yylineno); }
                ;

argument_list : extended_expr                      { $$ = new list<AST*>; $$->push_front($1); }
              | extended_expr COMMA argument_list  { $3->push_front($1); $$ = $3; }
              |                                    { $$ = new list<AST*>;}
              ;

function_stmt : CALL ID OPENPAREN argument_list CLOSEPAREN
               {
                    // printf("%d: Invoking function {%s(AST, AST ...)} as statement.\n", yylineno, $2);
                    //$$ = new AST(yylineno, $1, $3);
                    { $$ = new EmptyStatement(yylineno); }
               }
              ;
function_call : ID OPENPAREN argument_list CLOSEPAREN
               {
                    // printf("%d: Invoking function {%s(AST, AST ...)} as expression.\n", yylineno, $1);
                    $$ = new AST(yylineno, $1, $3);
               }
              ;

print : PRINT OPENPAREN argument_list CLOSEPAREN
          { $$ = new PrintStatement(yylineno, $3); }
      ;

eval : EVAL OPENPAREN extended_expr CLOSEPAREN { $$ = new Eval(yylineno, $3); }
     ;    
type_of : TYPE_OF OPENPAREN extended_expr CLOSEPAREN
     { $$ = new TypeOf(yylineno, $3); }


return_statement: RETURN expr { $$ = new EmptyStatement(yylineno); }
                | RETURN      { $$ = new EmptyStatement(yylineno); }
                ;

statement : assignment SEMICOLON
          | if_statement
          | for_statement
          | while_statement
          | print SEMICOLON
          | return_statement SEMICOLON
          | function_stmt SEMICOLON
          | eval SEMICOLON
          | type_of SEMICOLON
          | OPENBRACKET statements CLOSEBRACKET { $$ = new EmptyStatement(yylineno); }
          | SEMICOLON                           { $$ = new EmptyStatement(yylineno); }
          ;

statements : statements statement  { $1->push_back($2); $$ = $1; }
           | { $$ = new list<Statement*>(); }
           ;

code_block : OPENBRACKET global_variables statements CLOSEBRACKET { $$ = $3; }
           ;

function_definition : FUNCTION_DEF ID OPENPAREN parameters CLOSEPAREN COLON TYPE code_block
     { SymbolTable::getInstance()->addFunction($7, $2, $4, $8, yylineno); }
                    ; 

function_definitions : function_definitions function_definition
                     |
                     ;
%%

void yyerror(const char * s)
{
     cout << "error: " <<  s << " at line:" << yylineno << endl;
}

int main(int argc, char** argv)
{
     yyin=fopen(argv[1],"r");
     yyparse();

     cout << "Variables:" << endl;
     SymbolTable::getInstance()->printVars();  

     Function* mainF = SymbolTable::getInstance()->getFunction("main");
     if (mainF == nullptr)
     {
          cout << "main() function fot found!" << endl;
          return -1;
     }
     mainF->execute();  
} 