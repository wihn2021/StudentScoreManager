#pragma once
#include "AbstractCourse.h"
class AbstractCourseManager
{
protected:
	std::vector<AbstractCourse*> courses;
public:
	virtual AbstractCourse* FindCoursePtrByID(ItemIDType) = 0;
};

