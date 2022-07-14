#include "Score.h"

AbstractCourse* Score::getCourse()
{
    return co;
}

AbstractStudent* Score::getStudent()
{
    return stu;
}

int Score::getValue()
{
    return Value;
}

void Score::Serialize(std::ostream& op)
{
    op << stu->stuid << std::endl;
    op << co->ID << std::endl;
    op << Value << std::endl;
}

void Score::DisSerialize(std::istream& ip)
{
    long long tsid;
    long tid;
    ip >> tsid;
    
}

void Score::setValue(int _1)
{
    Value = _1;
}
