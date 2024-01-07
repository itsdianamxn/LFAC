#ifndef __VALUE
#define __VALUE

#include <string>
#include <map>
#include <iostream>

using namespace std;

class Value
{
public:
    virtual Value*  sum(Value* x) = 0;
    virtual Value*  mult(Value* x) = 0;
    virtual Value*  div(Value* x) = 0;
    virtual Value*  minus(Value* x) = 0;

    virtual int    intValue() = 0;
    virtual bool   boolValue() = 0;
    virtual float  floatValue() = 0;
    virtual char   charValue() = 0;
    virtual string stringValue() = 0;

    virtual string getType() = 0;
};

class IntValue : public Value
{
private:
    int val;
public:
    IntValue(int val): val(val) {}
    int    intValue()   { return val; }
    bool   boolValue()  { return val != 0; }
    float  floatValue() { return val; }
    char   charValue()  { return val; }
    string stringValue()  { return to_string(val); }
    Value* sum(Value* x)   { return new IntValue(this->val + x->intValue()); }
    Value* mult(Value* x)  { return new IntValue(this->val * x->intValue()); }
    Value* div(Value* x)   { return new IntValue(this->val / x->intValue()); }
    Value* minus(Value* x) { return new IntValue(this->val - x->intValue()); }
    string getType() {return "int"; }
};

class BoolValue : public Value
{
private:
    bool val;
public:
    BoolValue(bool val): val(val) {}
    int    intValue()   { return val; }
    bool   boolValue()  { return val; }
    float  floatValue() { return val; }
    char   charValue()  { return val; }
    string stringValue() { return val ? "true" : "false"; }
    Value* sum(Value* x)   { return  new BoolValue(this->val || x->boolValue()); }
    Value* mult(Value* x)   { return new BoolValue(this->val && x->boolValue()); }
    Value* div(Value* x)    { return new BoolValue(this->val / x->boolValue()); }
    Value* minus(Value* x)  { return new BoolValue(this->val - x->boolValue()); }
    string getType() {return "bool"; }
};

class FloatValue : public Value
{
private:
    float val;
public:
    FloatValue(float val): val(val) {}
    int    intValue()   { return val; }
    bool   boolValue()  { return val != 0; }
    float  floatValue() { return val; }
    char   charValue()  { return val; }
    string stringValue() { return to_string(val); }
    Value* sum(Value* x)   { return new FloatValue(this->val + x->floatValue()); }
    Value* mult(Value* x)   { return new FloatValue(this->val * x->floatValue()); }
    Value* div(Value* x)    { return new FloatValue(this->val / x->floatValue()); }
    Value* minus(Value* x)  { return new FloatValue(this->val - x->floatValue()); }
    string getType() {return "float"; }
};


class CharValue : public Value
{
private:
    char val;
public:
    CharValue(char val): val(val) {}
    int    intValue()   { return val; }
    bool   boolValue()   { return val != 0; }
    float  floatValue()  { return val; }
    char   charValue()   { return val; }
    string stringValue()   { return string(1, val); }
    Value* sum(Value* x)    { return new CharValue(this->val + x->charValue()); }
    Value* mult(Value* x)   { return new CharValue(this->val * x->charValue()); }
    Value* div(Value* x)    { return new CharValue(this->val / x->charValue()); }
    Value* minus(Value* x)  { return new CharValue(this->val - x->charValue()); }
    string getType() {return "char"; }
};


class StringValue : public Value
{
private:
    string val;
public:
    StringValue(string val): val(val) {}
    int    intValue()   { return stoi(val); }
    bool   boolValue()  { return val == "true"; }
    float  floatValue() { return stof(val); }
    char   charValue()  { return '?'; }
    string stringValue() { return val; }
    Value* sum(Value* x)    { return new StringValue(this->val + x->stringValue()); }
    Value* mult(Value* x)   { return new StringValue("?"); }
    Value* div(Value* x)    { return new StringValue("?"); }
    Value* minus(Value* x)  { return new StringValue("?"); }
    string getType() {return "string"; }
};
#endif