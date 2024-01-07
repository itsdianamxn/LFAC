#ifndef __SYMBOLTABLE
#define __SYMBOLTABLE

#include <string>
#include <map>
#include <iostream>
#include "Value.h"

using namespace std;

class Variable {
private:
    string type;
    string name;
    int lineno;
    Value* value;
    bool constant;

public:
    Variable(const char* type, const char* name, int lineno);
    void setValue(Value* val);
    void setConstant();
    bool isConstant() { return constant; }
    int getLine() {return lineno;}
    Value* getValue();
    string getName(){return name;}
    string getType() { return type; }
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
    Variable* get(string name);
    Variable* addVar(const char* type, const char* name, int lineno);
    void printVars();
    ~SymbolTable();
};

#endif