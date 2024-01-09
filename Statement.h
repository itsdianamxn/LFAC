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
            cout << "line : " <<lineno << " ERR: Invalid condition in if statement." <<endl;
            cout << cond->asString() << " : " << cond->getType() << " = " << cond->getValue() << endl; 
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

class ComposedStatement : public Statement
{
private:
    list <Statement*>* stmts;
public:
    ComposedStatement(int lineno, list <Statement*>* stmts) : Statement(lineno), stmts(stmts){}
    ~ComposedStatement() {}

    void run()
    {
        auto it = stmts->begin();
        while (it != stmts->end())
        {
            Statement* stmt = *it;
            stmt->run();
            ++it;
        }
    }
};

class WhileStatement : public Statement
{
private:
    AST* cond;
    Statement* onTrue;
public:
    WhileStatement(int lineno, AST* cond, Statement* onTrue) : 
        Statement(lineno), cond(cond), onTrue(onTrue) {}
    ~WhileStatement() {}

    void run()
    {
        if (cond->getType() != "bool")
        {
            cout <<"line : " <<lineno << " ERR: Invalid condition in while statement." <<endl;
            exit(-11);
        }
        // cout << "WHILE cond = "<< cond->asString() << endl;
        while (cond->getValue()->boolValue())
        {
            onTrue->run();
        }
    }
};

#endif