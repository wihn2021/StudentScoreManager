#pragma once
#include "AbstractStudent.h"
class Student :
    public AbstractStudent
{
public:
	Student(long long id, std::string name) : AbstractStudent(id, name) {};
	Student() {};
	void Serialize(std::ostream&);
	void DisSerialize(std::istream&);
	void AddAScore(AbstractScore*);
	void RemoveScore(AbstractScore*);
};

