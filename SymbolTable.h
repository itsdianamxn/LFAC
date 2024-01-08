#ifndef __SYMBOLTABLE
#define __SYMBOLTABLE

#include <string>
#include <map>
#include <list>
#include <iostream>
#include "Value.h"
#include "Statement.h"

using namespace std;

class Parameter {
private:
    string type;
    string name;
    int line;
public:
    Parameter(int line, const char* name, const char* type) : line(line), type(type), name(name) {}
};

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
    string getName() {return name;} 

    Value* getValue();
    string getType() { return type; }
};

class Function
{
private:
    string name;
    string return_type;
    list<Statement*>* stmts;
    list<Parameter*>* params;
    int lineno;

public:
    Function(string name, string return_type, list<Parameter*>* params, list <Statement*>* stmts, int lineno):
        name(name), return_type(return_type), params(params), stmts(stmts), lineno(lineno) {}
    string getType() {return return_type;} 
    int getLine() { return lineno; }
    Value* execute(); 
};

class SymbolTable
{
private:
    map<string, Variable*> vars;
    map<string, Function*> funcs;
    static SymbolTable* instance;
    Function* crtFunction = nullptr;
    SymbolTable();

public:
    static SymbolTable* getInstance();

    bool existsVar(const char* s);
    Variable* getVariable(string name);
    Variable* addVariable(const char* type, const char* name, int lineno);
    Function* getFunction(string name);
    Function* addFunction(const char* type, const char* name, list<Parameter*>* param, list<Statement*>* stmts, int lineno);
    

    void printVars();
    ~SymbolTable();
};

#endif