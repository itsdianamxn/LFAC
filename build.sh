#!/bin/bash

echo "compiling"
rm -f lex.yy.c
rm -f parser.tab.c
rm -f parser.tab.h
bison -d parser.y
flex lexer.l
g++ AST.cpp SymbolTable.cpp lex.yy.c parser.tab.c -std=c++11 -o compiler
