#pragma once
#include "AbstractScore.h"
#include "AbstractCourse.h"
#include "AbstractStudent.h"
class Score :
    public AbstractScore
{
public:
    Score(AbstractStudent* _1, AbstractCourse* _2, int _3)
    {
        stu = _1;
        co = _2;
        Value = _3;
    }
    Score() {};
    AbstractCourse* getCourse();
    AbstractStudent* getStudent();
    int getValue();
    void Serialize(std::ostream&);
    void DisSerialize(std::istream&);
    void setValue(int);
    bool operator==(const Score& e) const
    {
        if (Value == e.Value && co == e.co) return true;
        return false;
    }
    AbstractCourse* co;
    AbstractStudent* stu;
};

