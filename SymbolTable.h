#ifndef __SYMBOLTABLE
#define __SYMBOLTABLE

#include <string>
#include <map>
#include <iostream>
#include "Value.h"

using namespace std;

struct Variable {
// private:
    string type;
    string name;
    int lineno;
    Value* value;

public:
    Variable(const char* type, const char* name, int lineno);
    void setValue(Value* val);
    Value* getValue();
};

class SymbolTable
{
private:
    map<string, Variable*> vars;
    static SymbolTable* instance;
    SymbolTable(/* args */);

public:
    static SymbolTable* getInstance();

    bool existsVar(const char* s);
    Variable* get(const char* name);
    Variable* addVar(const char* type, const char* name, int lineno);
    void printVars();
    ~SymbolTable();
};

#endif