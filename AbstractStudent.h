#pragma once
#include "Object.h"
#include "AbstractCommon.h"
#include "AbstractScore.h"
class AbstractStudent :
    public Object
{
public:
	long long stuid;
	std::string name;
	virtual void AddAScore(AbstractScore*) = 0;
	AbstractStudent(long long stuid, std::string name) :stuid(stuid), name(name) {};
	AbstractStudent() {};
	std::vector<AbstractScore*> scores;
};