#pragma once
#include "AbstractStudent.h"
class AbstractStudentManager
{
protected:
	std::vector<AbstractStudent*> students;
public:
	virtual AbstractStudent* FindStudentPtrByStuid(long long) = 0;
};

