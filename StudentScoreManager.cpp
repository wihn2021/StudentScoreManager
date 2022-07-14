#include "common.h"
#include "StudentScoreManager.h"

const char* GPAString = "A+\0A\0A-\0B+\0B\0B-\0C+\0C\0C-\0D+\0F\0N/A";

enum class Grade
{
	A_PLUS=100,
	A=95,
	A_MINUS=90,
	B_PLUS=85,
	B=80,
	B_MINUS=77,
	C_PLUS=73,
	C=70,
	C_MINUS=67,
	D_PLUS=63,
	D=60,
	F=0,
	N=-1
};

Grade score2GPA(int sc)
{
	if (sc >= (int)Grade::A_PLUS) return Grade::A_PLUS;
	if (sc >= (int)Grade::A) return Grade::A;
	if (sc >= (int)Grade::A_MINUS) return Grade::A_MINUS;
	if (sc >= (int)Grade::B_PLUS) return Grade::B_PLUS;
	if (sc >= (int)Grade::B) return Grade::B;
	if (sc >= (int)Grade::B_MINUS) return Grade::B_MINUS;
	if (sc >= (int)Grade::C_PLUS) return Grade::C_PLUS;
	if (sc >= (int)Grade::C) return Grade::C;
	if (sc >= (int)Grade::C_MINUS) return Grade::C_MINUS;
	if (sc >= (int)Grade::D_PLUS) return Grade::D_PLUS;
	if (sc >= (int)Grade::D) return Grade::D;
	if (sc >= (int)Grade::F) return Grade::F;
	return Grade::N;
}

int StudentScoreManager::SaveToDB(std::ostream& op)
{
	/*
	* 这里把三个vector中的数据持久化存储，顺序依次为学生、课程、成绩。
	* 每次调用一个对象的序列化和反序列化函数，当然，这里写成运算符重载也可以。
	* 下面的load函数就是逆过程了，两个函数对照着非常整齐。
	*/
	int tempInt;
	tempInt = students.size();
	op<<tempInt<<std::endl;
	for (auto i : students) i->Serialize(op);
	tempInt = courses.size();
	op << tempInt << std::endl;
	for (auto i : courses) i->Serialize(op);
	tempInt = scores.size();
	op << tempInt << std::endl;
	for (auto i : scores) i->Serialize(op);
	return 0;
}

int StudentScoreManager::loadFromDB(std::istream& ip)
{
	int tempInt;
	ip >> tempInt;
	for (int i = 0; i < tempInt; i++)
	{
		auto p = new Student();
		p->DisSerialize(ip);
		addStudentWithoutCheck(p);
	}
	ip >> tempInt;
	for (int i = 0; i < tempInt; i++)
	{
		auto p = new Course();
		p->DisSerialize(ip);
		addCourseWithoutCheck(p);
		AutoCountable::IdIterator = AutoCountable::IdIterator < p->ID ? p->ID : AutoCountable::IdIterator;
	}
	ip >> tempInt;
	for (int i = 0; i < tempInt; i++)
	{
		long long tsid;
		long tcid;
		int val;
		ip >> tsid >> tcid >>val;
		auto p = new Score(FindStudentPtrByStuid(tsid), FindCoursePtrByID(tcid), val);
		addScoreWithoutCheck(p);
	}
	return 0;
}

int StudentScoreManager::addStudent(AbstractStudent* p)
{
	for (auto i : students)
	{
		if (i->stuid == p->stuid)
		{
			delete p;
			return 1;
		}
	}
	addStudentWithoutCheck(p);
	return 0;
}

int StudentScoreManager::addStudentWithoutCheck(AbstractStudent* p)
{
	students.push_back(p);
	return 0;
}

int StudentScoreManager::addCourseWithoutCheck(AbstractCourse* p)
{
	courses.push_back(p);
	return 0;
}

int StudentScoreManager::addCourse(AbstractCourse* p)
{
	for (auto i : courses)
	{
		if (i->name == p->name)
		{
			delete p;
			return 1;
		}
	}
	addCourseWithoutCheck(p);
	return 0;
}

int StudentScoreManager::addScore(Score* p)
{
	for (auto i : scores)
	{
		if (*i == *p)
		{
			i->setValue((p->getValue()));
			delete p;
			return 1;
		}
	}
	addScoreWithoutCheck(p);
	return 0;
}

int StudentScoreManager::addScoreWithoutCheck(Score* p)
{
	scores.push_back(p);
	p->getStudent()->AddAScore(p);
	p->getCourse()->AddAScore(p);
	return 0;
}

AbstractCourse* StudentScoreManager::FindCoursePtrByID(long id)
{
	for (auto i : courses)
	{
		if (i->ID == id) return i;
	}
	return nullptr;
}

AbstractCourse* StudentScoreManager::FindCoursePtrByName(std::string n)
{
	for (auto i : courses) if (i->name == n) return i;
	return nullptr;
}

AbstractStudent* StudentScoreManager::FindStudentPtrByStuid(long long id)
{
	for (auto i : students)
	{
		if (i->stuid == id)
		{
			return i;
		}
	}
	return nullptr;
}

int StudentScoreManager::deleteStudent(AbstractStudent* p)
{
	std::vector<AbstractStudent*>::iterator it;
	std::vector<Score*>::iterator sit;
	for (it = students.begin(); it != students.end(); it++)
	{
		if (*it == p) break;
	}
	for (auto s : p->scores)
	{
		for (sit = scores.begin(); sit != scores.end(); sit++) if ((void*)(*sit) == (void*)s) break;
		std::vector<AbstractScore*>::iterator cit;
		for (cit = (*sit)->getCourse()->scores.begin(); cit != (*sit)->getCourse()->scores.end(); cit++)
		{
			if ((void*)*cit == (void*)s) break;
		}
		(*sit)->getCourse()->scores.erase(cit);
		delete* sit;
		scores.erase(sit);
	}
	delete* it;
	students.erase(it);
	return 0;
}

int StudentScoreManager::deleteCourse(AbstractCourse* p)
{
	std::vector<AbstractCourse*>::iterator it;
	std::vector<Score*>::iterator sit;
	for (it = courses.begin(); it != courses.end(); it++)
	{
		if (*it == p) break;
	}
	for (auto s : p->scores)
	{
		for (sit = scores.begin(); sit != scores.end(); sit++) if ((void*)*sit == (void*)s) break;
		std::vector<AbstractScore*>::iterator cit;
		for (cit = (*sit)->getStudent()->scores.begin(); cit != (*sit)->getStudent()->scores.end(); cit++)
		{
			if ((void*)*cit == (void*)s) break;
		}
		(*sit)->getStudent()->scores.erase(cit);
		delete* sit;
		scores.erase(sit);
	}
	delete* it;
	courses.erase(it);
	return 0;
}

int StudentScoreManager::deleteScore(Score* p)
{
	std::vector<AbstractScore*>::iterator it;
	for (it = p->co->scores.begin(); it != p->co->scores.end(); it++)
	{
		if (*it == p) break;
	}
	p->co->scores.erase(it);
	for (it = p->stu->scores.begin(); it != p->stu->scores.end(); it++)
	{
		if (*it == p) break;
	}
	p->stu->scores.erase(it);
	std::vector<Score*>::iterator sit;
	for (sit = scores.begin(); sit != scores.end(); sit++)
	{
		if (*sit == p) break;
	}
	scores.erase(sit);
	delete p;
	return 0;
}

Score* StudentScoreManager::findScore(long long stuid, long cid)
{
	for (auto i : scores)
	{
		if (i->co->ID == cid && i->stu->stuid == stuid) return i;
	}
	return nullptr;
}
