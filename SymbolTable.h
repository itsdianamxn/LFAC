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
    string getType() { return type; }
    string getName() { return name; }
    int getLine() {return line;}
};

class Variable {
private:
    string type;
    string name;
    int lineno;
    Value* value;
    bool constant;

public:
    Variable(string type, string name, int lineno);
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
    map<string, Variable*> local_vars;
    int lineno;
    Value* retVal = nullptr;

public:
    Function(string name, string return_type, list<Parameter*>* params, list <Statement*>* stmts, int lineno):
        name(name), return_type(return_type), params(params), stmts(stmts), lineno(lineno) {}
    string getType() {return return_type;} 
    string getName() { return name; }
    int getLine() { return lineno; }
    list<Parameter*>* getParameters() { return params; }
    map<string, Variable*> getLocals() { return local_vars;}
    Value* execute(list<AST*>* args); 
    void setReturnValue(Value* v);
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

    Function* setCurrentFunction(Function* f);
    void setReturnValue(Value* v);

    void printVars();
    ~SymbolTable();
};

#endif