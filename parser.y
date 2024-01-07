%{
#include <iostream>
#include <vector>
#include <list>

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
     
}

%start progr
%token VARIABLE_DEF OBJECT_DEF FUNCTION_DEF FOR IF ELSE THEN WHILE PRINT RETURN
%token ASSIGN PLUS MINUS MULT DIV EQUAL LESS GREATER LESSEQUAL GREATEREQUAL NOTEQUAL
%token COLON SEMICOLON COMMA DOT DOT2
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
%type <node>    literal factor term expr extended_expr function_call
%type <astList> argument_list
%%

progr: user_data_types global_variables function_definitions {printf("The program is correct!\n");}
     ;

parameter:  ID COLON TYPE;

parameters : parameter 
           | parameters COMMA parameter
           |
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
                         { SymbolTable::getInstance()->addVar($4, $2, yylineno);  }
                | VARIABLE_DEF ID COLON ID OPENPAREN argument_list CLOSEPAREN SEMICOLON
                         { cout << "Define userdef variable " << $2 << " of type " << $4 << endl; }
                | VARIABLE_DEF ID COLON TYPE OPENPAREN literal CLOSEPAREN SEMICOLON
                         { Variable* v = SymbolTable::getInstance()->addVar($4, $2, yylineno);
                           v->setValue($6->getValue()); }
                | VARIABLE_DEF ID COLON TYPE OPENSQUARE INT_LITERAL CLOSESQUARE SEMICOLON
                         { cout << "Define array variable " << $2 << " of type " << $4 << endl;}
                ;

global_variables : global_variables global_variable
                 |
                 ;

factor: literal
     | ID OPENSQUARE extended_expr CLOSESQUARE    // access array element
               {  $$ = new AST(yylineno, $1, $3); }
     | ID DOT ID OPENPAREN argument_list CLOSEPAREN // accessing object method
               {
                    printf("%d: Accessing object method {%s.%s(AST, AST ...)}.\n", yylineno, $1, $3);
                    $$ = new AST(yylineno, $1, $3, $5);
               }
     | ID DOT ID OPENSQUARE extended_expr CLOSESQUARE     // access array element member of an objet
               { printf("%d: Accessing object array field {%s.%s[AST]}.\n", yylineno, $1, $3); }
     | ID DOT ID                                       
               { 
                    printf("%d Accessing object filed {%s.%s}.\n", yylineno, $1, $3);
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
              | expr LESSEQUAL expr      { $$ = new AST(yylineno, $1, $3, 'l'); }
              | expr GREATEREQUAL expr   { $$ = new AST(yylineno, $1, $3, 'g'); }
              | expr NOTEQUAL expr       { $$ = new AST(yylineno, $1, $3, '!'); }
              ;

assignment : ID ASSIGN extended_expr { cout << yylineno << " ASSIGN " << $1 << " <- " << "$3" << endl; }
           ;

if_statement : IF OPENPAREN extended_expr CLOSEPAREN THEN statement
                    { cout << yylineno << ": IF <expr> THEN <stmt>"  << endl; }
             | IF OPENPAREN extended_expr CLOSEPAREN THEN statement ELSE statement
                    { cout << yylineno << ": IF <expr> THEN <stmt> ELSE <stmt>"  << endl; }
             ;

for_statement : FOR OPENPAREN global_variable SEMICOLON extended_expr SEMICOLON extended_expr CLOSEPAREN statement
              ;

while_statement : WHILE OPENPAREN extended_expr CLOSEPAREN statement
                ;

argument_list : extended_expr                      { $$ = new list<AST*>; $$->push_front($1); }
              | extended_expr COMMA argument_list  { $3->push_front($1); $$ = $3; }
              |                                    { $$ = new list<AST*>;}
              ;

function_call : ID OPENPAREN argument_list CLOSEPAREN
               {
                    printf("%d: Invoking function {%s(AST, AST ...)}.\n", yylineno, $1);
                    $$ = new AST(yylineno, $1, $3);
               }
              ;

print : PRINT OPENPAREN argument_list CLOSEPAREN { cout << yylineno << ": PRINT() " << endl; }
      ;

return_statement: RETURN expr
                | RETURN
                ;

statement : assignment SEMICOLON
          | if_statement
          | for_statement
          | while_statement
          | print SEMICOLON
          | return_statement SEMICOLON
          | function_call SEMICOLON
          | OPENBRACKET statements CLOSEBRACKET
          | SEMICOLON
          ;

statements : statements statement
           |
           ;

code_block : OPENBRACKET global_variables statements CLOSEBRACKET
           ;

function_definition : FUNCTION_DEF ID OPENPAREN parameters CLOSEPAREN COLON TYPE code_block
                    ; 

function_definitions : function_definitions function_definition
                     |
                     ;
%%

void yyerror(const char * s) {
     printf("error: %s at line:%d\n",s,yylineno);
}

int main(int argc, char** argv) {
     yyin=fopen(argv[1],"r");
     yyparse();
     cout << "Variables:" << endl;
     SymbolTable::getInstance()->printVars();  
} 