#pragma once
#include "Object.h"
class AutoCountable :
    public Object
{
public:
	static long IdIterator;
	long ID;
	AutoCountable()
	{
		ID = IdIterator++;
	}
	void setID(long id)
	{
		ID = id;
	}
};

