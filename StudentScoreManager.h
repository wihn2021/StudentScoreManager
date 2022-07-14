#pragma once
#include "AbstractStudentScoreManager.h"
#include "Student.h"
#include "Course.h"
#include "Score.h"

class StudentScoreManager :
	public AbstractStudentScoreManager
{
public:
	int SaveToDB(std::ostream&);
	int loadFromDB(std::istream&);
	int addStudent(AbstractStudent*);
	int addStudentWithoutCheck(AbstractStudent*);
	int addCourseWithoutCheck(AbstractCourse*);
	int addCourse(AbstractCourse*);
	int addScore(Score*);
	int addScoreWithoutCheck(Score*);
	AbstractCourse* FindCoursePtrByID(long);
	AbstractCourse* FindCoursePtrByName(std::string);
	AbstractStudent* FindStudentPtrByStuid(long long);
	int deleteStudent(AbstractStudent*);
	int deleteCourse(AbstractCourse*);
	int deleteScore(Score*);
	Score* findScore(long long, long);
	StudentScoreManager() {};
};