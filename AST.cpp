#include "AST.h"
#include "SymbolTable.h"


AST::AST(int lineno, AST* left, AST* right, char op) :
    op(op), lineno(lineno), left(left), right(right), leaf(nullptr)
{
    // an inner node
}

AST::AST(int lineno, Value* val): 
    op(AST_LITERAL), lineno(lineno), left(nullptr), right(nullptr), leaf(val)
{
    // leaf literal
}

AST::AST(int lineno, const char* id): 
    op(AST_IDENTIFIER), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id)
{
    // identifier (variable/ parameter) leaf
}

AST::AST(int lineno, const char* id, AST* index): 
    op(AST_ARRAY_IDENTIFIER), lineno(lineno), right(nullptr), left(index), ident(id)
{
    // array identifier (variable/ parameter) leaf
    // using [left] to store the index tree
}

AST::AST(int lineno, const char* fn, list<AST*>* params):
    op(AST_FUNCTION_CALL), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(fn), params(params)
{
    // function call
}

AST::AST(int lineno, const char* id, const char* meth, list<AST*>* params) :  
    op(AST_METHOD), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id), method(meth), params(params)
{
     // method call leaf
}

AST::AST(int lineno, const char* id, const char* field) :
    op(AST_FIELD), lineno(lineno), left(nullptr), right(nullptr), leaf(nullptr), ident(id), method(field)
{
    // field access
}

AST::~AST(){}


string AST::getType()
{
    switch (op)
    {
        case AST_LITERAL: return leaf == nullptr ? "void" : leaf->getType();
        case AST_IDENTIFIER:
        {
            Variable* v = SymbolTable::getInstance()->getVariable(ident);
            if(v==nullptr)
            {
                cout <<"line : " <<lineno << " ERR: missing variable '" << ident << "'." <<endl;
                exit(-7);
            }
            return v->getType();
        }
        case AST_FUNCTION_CALL:
        {
            Function* f = SymbolTable::getInstance()->getFunction(ident);
            if(f==nullptr)
            {
                cout <<"line : " <<lineno << " ERR: undeclared function '" << ident << "'." <<endl;
                exit(-7);
            }
            return f->getType();
        }
        case AST_ARRAY_IDENTIFIER:
            return "void"; // TODO implement me
        case AST_METHOD:
            return "void"; // TODO implement me
        case AST_FIELD:
            return "void"; // TODO implement me
        default:
        {
            if (left == nullptr)
            {
                cout <<"line : " <<lineno << " ERR: missing left node"<<endl;
                return nullptr;
            }

            if (right == nullptr)
            {
                cout <<"line : " <<lineno << " ERR: missing right node"<<endl;
                return nullptr;
            }
            string leftType = left->getType();
            string rightType = right->getType();
            if (leftType != rightType)
            {
                cout <<"line : " <<lineno << " ERR: nodes' types not matching"<<endl;
                return nullptr;
            }
            if (op == AST_GEQ || op == AST_NOT_EQ || op ==AST_LEQ || op == '>' || op == '<' || op == '=')
                return "bool";
            return leftType;
        }
    }
}

Value* AST::getValue()
{
    switch (op)
    {
        case AST_LITERAL: return leaf;
        case AST_IDENTIFIER:
        {
            Variable* v = SymbolTable::getInstance()->getVariable(ident);
            if (v==nullptr)
            {
                cout <<"line : " <<lineno << " ERR: missing variable '" << ident << "'." <<endl;
                exit(-7);
            }
            return v->getValue();
        }
        case AST_FUNCTION_CALL:
        {
            Function* f = SymbolTable::getInstance()->getFunction(ident);
            if (f==nullptr)
            {
                cout <<"line : " <<lineno << " ERR: undeclared function '" << ident << "'." <<endl;
                exit(-7);
            }
            return f->execute(params);
        }
        case AST_ARRAY_IDENTIFIER:
            return 0; // TODO implement me
        case AST_METHOD:
            return 0; // TODO implement me
        case AST_FIELD:
            return 0; // TODO implement me
        default:
        {
            if (left == nullptr)
            {
                cout <<"line : " <<lineno <<" ERR: missing left node"<<endl;
                return nullptr;
            }

            if (right == nullptr)
            {
                cout <<"line : " <<lineno <<" ERR: missing right node"<<endl;
                return nullptr;
            }
            string leftType = left->getType();
            string rightType = right->getType();
            if (leftType != rightType)
            {
                cout <<"line : " <<lineno << " ERR: nodes' types not matching"<<endl;
                return nullptr;
            }
            Value* leftValue = left->getValue();
            Value* rightValue = right->getValue();

            switch (op)
            {
                case '+': return leftValue->sum(rightValue);
                case '-': return leftValue->minus(rightValue);
                case '*': return leftValue->mult(rightValue);
                case '/': return leftValue->div(rightValue);
                
                case '<': return leftValue->isGreaterThan(rightValue);
                case '=': return leftValue->isEqual(rightValue);
                case '>': return leftValue->isLessThan(rightValue);
                case AST_NOT_EQ: 
                {
                    Value* eq = leftValue->isEqual(rightValue);
                    return new BoolValue(!eq->boolValue());
                }
                case AST_GEQ: return new BoolValue(!(leftValue->isLessThan(rightValue))->boolValue());
                case AST_LEQ: return new BoolValue(!(leftValue->isGreaterThan(rightValue))->boolValue());

                default:
                    return nullptr;
            }
        }
    }
}

string AST::asString()
{
    switch (op)
    {
        case AST_IDENTIFIER: return ident;
        case AST_LITERAL: return leaf->stringValue();
        default: 
            string ret = "(";
            if (left != nullptr) ret += left->asString() + " ";
            ret += op;
            if (right != nullptr) ret += " " + right->asString();
            ret += ")";
        return ret;
    }
}