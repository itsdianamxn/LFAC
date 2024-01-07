#ifndef __STATEMENT
#define __STATEMENT

class Statement
{
protected:
    int lineno;
public:
    Statement(int lineno): lineno(lineno) {}
    ~Statement() {};
    virtual void run() = 0;
};


#endif