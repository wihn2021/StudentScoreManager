#include "Student.h"

void Student::Serialize(std::ostream& op)
{
	op << stuid << std::endl;
	op << name << std::endl;
}

void Student::DisSerialize(std::istream& ip)
{
	ip >> stuid;
	ip >> name;
}

void Student::AddAScore(AbstractScore* p)
{
	scores.push_back(p);
}
