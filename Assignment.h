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
        Variable* v = SymbolTable::getInstance()->getVariable(id);
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
            cout <<"line : " <<lineno << " ERR: mismatched types lvalue: " << v->getType() <<
                " rvalue: "<<  rightType << "." <<endl;
            exit(-8);
        }
        Value* val = ast->getValue();
        // cout << "\tAssinging " << val->stringValue() << " to " << v->getName() << endl;
        v->setValue(val);
        // cout << "\tValue of " << v->getName() << " is now " << v->getValue()->stringValue() << endl;
        
    }
};

#endif