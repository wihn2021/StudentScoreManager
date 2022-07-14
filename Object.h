#pragma once
#define ItemIDType long
#include "AbstractCommon.h"
class Object
{

public:
	virtual void Serialize(std::ostream&)=0;
	virtual void DisSerialize(std::istream&) = 0;
};