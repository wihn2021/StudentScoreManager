#pragma once
#include "AutoCountable.h"
#include "AbstractCommon.h"
#include "AbstractScore.h"
class AbstractCourse :
    public AutoCountable
{
public:
	std::string name;
	virtual void AddAScore(AbstractScore*) = 0;
	AbstractCourse(std::string name) :name(name) {};
	AbstractCourse() {};
	AbstractCourse(long id, std::string name):name(name)
	{
		setID(id);
	}
	std::vector<AbstractScore*> scores;
};

