#include "Course.h"

void Course::Serialize(std::ostream& op)
{
	op << ID << std::endl;
	op << name << std::endl;
}

void Course::DisSerialize(std::istream& ip)
{
	ip >> ID;
	ip >> name;
}

void Course::AddAScore(AbstractScore* p)
{
	scores.push_back(p);
}
