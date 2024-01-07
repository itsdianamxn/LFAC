#ifndef __STATEMENT
#define __STATEMENT

#include <iostream>
#include "AST.h"

using namespace std;

class Statement
{
protected:
    int lineno;
public:
    Statement(int lineno): lineno(lineno) {}
    ~Statement() {};
    virtual void run() = 0;
};

class EmptyStatement : public Statement
{
public:
    EmptyStatement(int lineno) : Statement(lineno) {}
    void run() { cout << "Executed statement at line " << lineno << endl; }
};

class IfStatement : public Statement
{
private:
    AST* cond;
    Statement* onTrue;
    Statement* onFalse;
public:
    IfStatement(int lineno, AST* cond, Statement* onTrue, Statement* onFalse) : 
        Statement(lineno), cond(cond), onTrue(onTrue), onFalse(onFalse) {}
    ~IfStatement() {}

    void run()
    {
        if (cond->getType() != "bool")
        {
            cout <<"line : " <<lineno << " ERR: Invalid condition in if statement." <<endl;
            exit(-11);
        }
        if (cond->getValue()->boolValue())
        {
            onTrue->run();
        }
        else if (onFalse != nullptr)
        {
            onFalse->run();
        }
    }
};

#endif