#ifndef __RETURN_STATEMENT
#define __RETURN_STATEMENT

#include "AST.h"
#include "Statement.h"
#include "SymbolTable.h"

class ReturnStatement : public Statement
{
private:
    AST* retVal;
public:
    ReturnStatement(int lineno, AST* retVal) : Statement(lineno), retVal(retVal) {}
    ~ReturnStatement() {}

    void run()
    {
        SymbolTable::getInstance()->setReturnValue(retVal->getValue());
    }
};
#endif