#ifndef __PRINTSTATEMENT
#define __PRINTSTATEMENT
#include <list>
#include <iostream>
#include "AST.h"
#include "Statement.h"

using namespace std;
class PrintStatement : public Statement
{
private:
    list <AST*>* to_print;
public:
    PrintStatement(int lineno, list <AST*>* to_print): Statement(lineno), to_print(to_print) {} 
    ~PrintStatement() {}
    void run ()
    {

        auto it = to_print->begin();
        while (it != to_print->end())
        {
            AST* ast = *it;
            cout << ast->getValue()->stringValue();
            ++it;
        }
    }
};

#endif