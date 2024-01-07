#ifndef __AST
#define __AST

#include <string>
#include <map>
#include <list>
#include <iostream>
#include "Value.h"

#define AST_LITERAL         'L'
#define AST_IDENTIFIER 'I'
#define AST_FUNCTION_CALL   'F'
#define AST_ARRAY_IDENTIFIER 'A'
#define AST_METHOD 'M'
#define AST_FIELD  'f'
#define AST_GEQ    'g'
#define AST_LEQ    'l'
#define AST_NOT_EQ '!'

using namespace std;

class AST
{
private:
    AST* left;
    AST* right;
    int lineno;
    char op;
    Value* leaf;
    string ident;
    string method;
    list<AST*>* params = nullptr;

public:
    AST(int lineno, Value* id);                             // leaf with literal
    AST(int lineno, const char* name);                      // leaf with id (variable / parameter)
    AST(int lineno, const char* name, AST* index);          // leaf with id (variable / parameter) array
    AST(int lineno, const char* fn, list<AST*>* params);    // function_call

    AST(int lineno, const char* id, const char* field);                     // leaf with field access
    AST(int lineno, const char* id, const char* meth, list<AST*>* params);  // leaf with method call

    AST(int lineno, AST* left, AST* right, char op); // inner node
    ~AST();
    string getType(); 
    Value* getValue();
};

#endif