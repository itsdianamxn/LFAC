#ifndef __ASSIGNMENT
#define __ASSIGNMENT
#include <iostream>
#include <string>

#include "SymbolTable.h"
#include "AST.h"
#include "Statement.h"

using namespace std;
class Assignment : public Statement
{
private:
    string id;
    AST* ast ;
public:
    Assignment(int lineno, const string& id, AST* ast): Statement(lineno), id(id), ast(ast) {} 
    ~Assignment() {}
    void run ()
    {
        Variable* v = SymbolTable::getInstance()->get(id);
        if(v == nullptr)
        {
            cout <<"line : " <<lineno << " ERR: variable not declared." <<endl;
            exit(-10);
        }
        if(v->isConstant())
        {
            cout <<"line : " <<lineno << " ERR: constant cannot have value changed." <<endl;
            exit(-9);
        }
        string rightType = ast->getType();
        if (rightType != v->getType())
        {
            cout <<"line : " <<lineno << " ERR: mismatched types." <<endl;
            exit(-8);
        }
        v->setValue(ast->getValue());
    }
};

#endif