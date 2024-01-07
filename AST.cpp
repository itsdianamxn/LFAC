#include "AST.h"

AST::AST(int lineno, AST* left, AST* right, char op) :
    op(op), lineno(lineno), left(left), right(right), leaf(nullptr)
{
    // an inner node
}

AST::AST(int lineno, Value* val): 
    op('L'), lineno(lineno), left(nullptr), right(nullptr), leaf(val)
{
    // leaf literal
}

AST::AST(int lineno, const char* id): 
    op('I'), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id)
{
    // identifier (variable/ parameter) leaf
}

AST::AST(int lineno, const char* id, AST* index): 
    op('A'), lineno(lineno), right(nullptr), left(index), ident(id)
{
    // array identifier (variable/ parameter) leaf
    // using [left] to store the index tree
}

AST::AST(int lineno, const char* fn, list<AST*>* params):
    op('F'), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(fn), params(params)
{
    // function call
}

AST::AST(int lineno, const char* id, const char* meth, list<AST*>* params) :  
    op('M'), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id), method(meth), params(params)
{
     // method call leaf
}

AST::AST(int lineno, const char* id, const char* field) :
    op('f'), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id), method(field)
{
    // field access
}

AST::~AST(){}


string AST::getType()
{
    if (op == 'L')
    {
        return leaf == nullptr ? "void" : leaf->getType();
    }

// TODO: add other node types

    if (left == nullptr)
    {
        cout << "ERR: missing left node"<<endl;
        return nullptr;
    }

    if (right == nullptr)
    {
        cout << "ERR: missing right node"<<endl;
        return nullptr;
    }
    string leftType = left->getType();
    string rightType = right->getType();
    if (leftType != rightType)
    {
        cout<<"ERR: nodes' types not matching"<<endl;
        return nullptr;
    }
    return leftType;
}

Value* AST::getValue()
{
    if (op == 'L')
    {
        return leaf;
    }

// TODO: add other node types

    if (left == nullptr)
    {
        cout << "ERR: missing left node"<<endl;
        return nullptr;
    }

    if (right == nullptr)
    {
        cout << "ERR: missing right node"<<endl;
        return nullptr;
    }

    // compute
    return new IntValue(1);
}