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

Variable* SymbolTable::addVar(const char* type, const char* name, int lineno)
{
    printf("Adding variable %s of type %s at line %d\n", name, type, lineno);
    Variable* v = vars[name];
    if (v == nullptr)
    {
        v = new Variable(type, name, lineno);
        vars[name] = v;
    } 
    else
    {
        printf("ERR: Duplicate variable '%s', first at line %d, second at line %d.\n", name, v->lineno, lineno);
    }
    return v;
}

Variable* SymbolTable::get(const char* name)
{
    return vars[name];
}

void SymbolTable::printVars()
{
    // cout << "IntValue " << (new IntValue(0))->stringValue() << endl;
    // cout << "BoolValue " << (new BoolValue(false))->stringValue() << endl;
    // cout << "FloatValue " << (new FloatValue(0))->stringValue() << endl;
    // cout << "CharValue " << (new CharValue(0))->stringValue() << endl;
    // cout << "StringValue " << (new StringValue("x"))->stringValue() << endl;

    //map<string, int>::iterator it = vars.begin();
    auto it = vars.begin();

    // Iterate through the map and print the elements
    while (it != vars.end())
    {
        Variable* var = it->second;
        string stringValue = "(null)";
        if (var->getValue() != nullptr)
        {
            stringValue = var->getValue()->stringValue();
        }
        cout<< var->lineno <<"\t" << var->type << "\t" << var->name << "\t" << stringValue << endl;
        ++it;
    }
}

