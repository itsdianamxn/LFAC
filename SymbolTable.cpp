#include "SymbolTable.h"

Variable::Variable(const char* type, const char* name, int lineno): type(type), name(name), lineno(lineno)
{
    if (this->type == "int") value = new IntValue(0);
    else if (this->type == "bool") value = new BoolValue(false);
    else if (this->type == "float") value = new FloatValue(0.0);
    else if (this->type == "char") value = new CharValue('\0');
    else if (this->type == "string") value = new StringValue("");
    else value = nullptr;
}

void Variable::setValue(Value* val)
{
    value = val;
}

Value* Variable::getValue()
{
    return value;
}

void Variable::setConstant()
{
    this->constant = true;
}

/*static*/ SymbolTable* SymbolTable::instance = nullptr;

/*static*/ SymbolTable* SymbolTable::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SymbolTable();
    }

    return instance;
}

SymbolTable::SymbolTable() {}

SymbolTable::~SymbolTable() {}

bool SymbolTable::existsVar(const char* s)
{
    return vars[s]!=nullptr;
}

Variable* SymbolTable::addVariable(const char* type, const char* name, int lineno)
{
    // printf("Adding variable %s of type %s at line %d\n", name, type, lineno);
    Variable* v = vars[name];
    if (v == nullptr)
    {
        v = new Variable(type, name, lineno);
        vars[name] = v;
    } 
    else
    {
        cout <<"line : " <<lineno <<" ERR: Duplicate variable " << name << " already declared at line " << v->getLine() <<endl;
        exit(-6);
    }
    return v;
}
Variable* SymbolTable::getVariable(string name)
{
    return vars[name];
}
// Iterate through the map and print the elements
void SymbolTable::printVars()
{
    auto it = vars.begin();

    cout << "LineNo\tType\tVarName\tValue"<< endl;
    while (it != vars.end())
    {
        Variable* var = it->second;
        string stringValue = "(null)";
        if (var->getValue() != nullptr)
        {
            stringValue = var->getValue()->stringValue();
        }
        char is_const = var->isConstant()?'*':' ' ;
        cout<< var->getLine() <<"\t" << var->getType() <<is_const << "\t" << var->getName() << "\t" << stringValue << endl;
        ++it;
    }
}

Function* SymbolTable::addFunction(const char* type, const char* name, list<Parameter*>* params, list<Statement*>* stmts, int lineno)
{
    Function* f = funcs[name];
    if (f == nullptr)
    {
        funcs[name] = new Function(name, type, params, stmts, lineno);
    }
    else
    {
        cout << "line : " << lineno << " ERR: Duplicate function " << name << 
                " already declared at line " << f->getLine() <<endl;
        exit(-5);
    }
    return f;
}

Function* SymbolTable::getFunction(string name)
{
    return funcs[name];
} 

Value* Function::execute()
{
    // match arguments with parameters
    // evaluate arguments
    // add arguments as variables
    
    // execute statements
    auto it = stmts->begin();
    while (it != stmts->end())
    {
        Statement* stmt = *it;
        stmt->run();
        ++it;
    }
    return nullptr;
}