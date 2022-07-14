#pragma once
#include "Object.h"

class AbstractScore :
    public Object
{
protected:
    int Value;
    
public:
    virtual int getValue() = 0;

    virtual void setValue(int) = 0;
};

