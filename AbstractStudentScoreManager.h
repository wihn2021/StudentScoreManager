#pragma once
#include <vector>
#include "AbstractStudentManager.h"
#include "AbstractCourseManager.h"
#include "AbstractScoreManager.h"
class AbstractStudentScoreManager:
	public AbstractStudentManager, public AbstractCourseManager, public AbstractScoreManager
{
public:
	virtual int SaveToDB(std::ostream&) = 0;
	virtual int loadFromDB(std::istream&) = 0;
	virtual int addStudent(AbstractStudent*) = 0;
	virtual int addStudentWithoutCheck(AbstractStudent*) = 0;
	virtual int addCourse(AbstractCourse*) = 0;
	virtual int addCourseWithoutCheck(AbstractCourse*) = 0;
};

