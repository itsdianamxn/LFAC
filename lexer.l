%{
#include <stdio.h>
#include <string.h>
#include "Value.h"
#include "AST.h"

#include "Statement.h"
#include "Assignment.h"
#include "PrintStatement.h"
/*#include "SymbolTable.h"*/
#include "parser.tab.h"

%}

LETTER  [a-zA-Z]
DIGIT   [0-9]
%option noyywrap

%%

"int"|"bool"|"float"|"char"|"string"|"void" { yylval.string = strdup(yytext); return TYPE; }
"true"      { yylval.boolean = true; return TRUE;}
"false"     { yylval.boolean = false; return FALSE;}
"var"       {return VARIABLE_DEF;}
"const"     {return CONST; }
"call"      {return CALL; }
"object"    {return OBJECT_DEF;}
"function"  {return FUNCTION_DEF;}
"for"       {return FOR;}
"if"        {return IF;}
"else"      {return ELSE;}
"then"      {return THEN;}
"while"     {return WHILE;}
"print"     {return PRINT;}
"return"    {return RETURN;}

\=  { return ASSIGN; }
\+  { return PLUS; }
-   { return MINUS; }
\*  { return MULT; }
\/  { return DIV; }
\=\= { return EQUAL; }
\<  { return LESS; }
\>  { return GREATER; }
\<\= { return LESSEQUAL; }
\>\= { return GREATEREQUAL; }
\<\> { return NOTEQUAL; }
\!\= { return NOTEQUAL; }
\(  { return OPENPAREN; }
\)  { return CLOSEPAREN; }
\{  { return OPENBRACKET; }
\}  { return CLOSEBRACKET; }
\[  { return OPENSQUARE; }
\]  { return CLOSESQUARE; }
\.  { return DOT; }
"#"  { return DOT2; }
\,  { return COMMA; }
\:  { return COLON;}
\;  { return SEMICOLON;}

{LETTER}({LETTER}|{DIGIT}|_)*   { yylval.string = strdup(yytext); return ID;}
-?{DIGIT}+\.{DIGIT}+            { yylval.real = atof(yytext); return FLOAT_LITERAL;} 
-?{DIGIT}+                      { yylval.integer = atoi(yytext);  return INT_LITERAL;}
"/*"([^*]|(\*+[^*/]))*\*+\/     {continue; /* Skip comments. */}
\"(\\.|[^"])*\"                 { yylval.string = strndup(yytext + 1, strlen(yytext) - 2); return STRING_LITERAL;}
\'.\'                           { yylval.string= strndup(yytext + 1, 1); return CHAR_LITERAL;}
[ \r\t]*    { continue; /* Skip blanks. */ }
\n          {yylineno++;}
. { printf("WARN: ignoring '%c'", yytext[0]); return yytext[0];}
%%