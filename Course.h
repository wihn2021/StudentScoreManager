#pragma once
#include "AbstractCourse.h"
class Course :
    public AbstractCourse
{
public:
	Course(std::string n) :AbstractCourse(n) {};
	Course(long id, std::string n) :AbstractCourse(id, n) {};
	Course() {};
	void Serialize(std::ostream&);
	void DisSerialize(std::istream&);
	void AddAScore(AbstractScore*);
};

