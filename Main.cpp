#include "StudentScoreManager.h"
#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>

StudentScoreManager ssm;
int load()
{
	std::ifstream ip;
	try
	{
		ip.open("ssm.db", std::ios_base::in);
		ssm.loadFromDB(ip);
	}
	catch (std::exception& e)
	{
		printf("LOAD ERROR. Creating a new database...\n");
	}
	ip.close();
	return 0;
}
int save()
{
	std::ofstream op;
	try
	{
		op.open("ssm.db", std::ios_base::out | std::ios_base::beg);
		ssm.SaveToDB(op);
	}
	catch (const std::exception&)
	{
		std::cout << "SAVE DATABASE ERROR\n";
		getchar();
		return 0;
	}
	op.close();
	return 0;
}
int importStudentInfo()
{
	printf("�������ļ����ơ��ļ��ڸ�ʽӦΪ��ѧ��+�ո�+����\n>>> ");
	std::string filename;
	std::cin >> filename;
	std::ifstream ip;
	try
	{
		ip.open(filename, std::ios_base::in);
		if (!ip.is_open())
		{
			throw "FileNotFoundError";
		}
		long long id;
		std::string name;
		while (!ip.eof())
		{
			ip >> id >> name;
			Student* student = new Student(id, name);
			ssm.addStudent(student);
		}
	}
	catch (const std::exception&)
	{
		printf("�������\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
		return 1;
	}
	printf("ѧ����Ϣ����ɹ�\n");
	return 0;
}
int importCourseInfo()
{
	printf("�������ļ������ļ���һ��һ���γ���\n>>> ");
	std::string filename;
	std::cin >> filename;
	std::ifstream ip;
	try
	{
		ip.open(filename, std::ios_base::in);
		if (!ip.is_open())
		{
			throw "FileNotFoundError";
		}
		std::string name;
		while (!ip.eof())
		{
			ip >> name;
			Course* course = new Course(name);
			ssm.addCourse(course);
		}
	}
	catch (const std::exception&)
	{
		printf("�������\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
	}
	printf("�γ���Ϣ����ɹ�\n");
	return 0;
}
int importCourseAndScore()
{
	printf("�������ļ������ļ���ӦΪ��ѧ��+�ո�+�γ���+�ո�+�ɼ�\n>>> ");
	std::string filename;
	std::cin >> filename;
	std::ifstream ip;
	try
	{
		ip.open(filename, std::ios_base::in);
		if (!ip.is_open())
		{
			throw "FileNotFoundError";
		}
		long long stuid;
		std::string name;
		int score;
		while (!ip.eof())
		{
			ip >> stuid >> name >> score;
			AbstractCourse* co = ssm.FindCoursePtrByName(name);
			AbstractStudent* st = ssm.FindStudentPtrByStuid(stuid);
			if (co == nullptr || st == nullptr)
			{
				throw "Course/StudentNotExistError";
			}
			else
			{
				Score* s = new Score(st, co, score);
				ssm.addScore(s);
			}
		}
	}
	catch (const std::exception&)
	{
		printf("�������\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
	}
	printf("�ɼ�����ɹ�\n");
	return 0;
}
int deleteStudent()
{
	printf("������ѧ��ѧ����ɾ��������صĳɼ����޷�������\n>>> ");
	long long stuid;
	std::cin >> stuid;
	auto t = ssm.FindStudentPtrByStuid(stuid);
	if (t != nullptr)
	{ 
		ssm.deleteStudent(t);
		printf("ɾ���ɹ�\n");
		return 0;
	}
	else printf("StudentNotExist\n");
	return 1;
}
int deleteCourse()
{
	printf("������γ�������ɾ��������ص���Ϣ���޷�������\n>>> ");
	std::string name;
	std::cin >> name;
	auto t = ssm.FindCoursePtrByName(name);
	if (t != nullptr)
	{
		ssm.deleteCourse(t);
		printf("ɾ���ɹ�\n");
		return 0;
	}
	else printf("CourseNotExist\n");
	return 1;
}
int deleteScore()
{
	printf("������ѧ��ѧ��+�ո�+�γ�����\n>>> ");
	long long stuid;
	std::string name;
	std::cin >> stuid >> name;
	AbstractCourse* c = ssm.FindCoursePtrByName(name);
	auto sp = ssm.findScore(stuid, c->ID);
	if (sp == nullptr)
	{
		printf("NoMatchingScore\n");
		return 1;
	}
	ssm.deleteScore(sp);
	printf("ɾ���ɹ�\n");
	return 0;
}
int QueryStudent()
{
	printf("������ѧ��ѧ�ţ�\n>>> ");
	long long s;
	try
	{
		std::cin >> s;
		auto p = ssm.FindStudentPtrByStuid(s);
		if (p == nullptr)
		{
			throw "StudentNotExist";
		}
		printf("������%s\nѧ�ţ�%lld\n", p->name.c_str(), p->stuid);
		for (auto i : p->scores)
		{
			printf("%10s%4d\n", ((Score*)i)->co->name.c_str(), ((Score*)i)->getValue());
		}
	}
	catch (std::exception& e)
	{
		printf("����ѧ������\n");
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
		return 1;
	}
	return 0;

}
int QueryCourse()
{
	printf("������γ�����\n>>> ");
	std::string s;
	std::cin >> s;
	auto c = ssm.FindCoursePtrByName(s);
	if (c == nullptr)
	{
		printf("CourseNotExist\n");
		return 1;
	}
	if (c->scores.size() == 0)
	{
		printf("NoScoreData\n");
		return 1;
	}
	int Max = 0, Min = 100, Sum = 0;
	double ave;
	printf("%10s %5s %4s\n", "ѧ��", "����", "�ɼ�");
	for (auto i : c->scores)
	{
		printf("%10lld %5s %4d\n", ((Score*)i)->stu->stuid, ((Score*)i)->stu->name.c_str(), i->getValue());
		if (i->getValue() > Max) Max = i->getValue();
		if (i->getValue() < Min) Min = i->getValue();
		Sum += i->getValue();
	}
	ave = Sum / c->scores.size();
	printf("====================\nͳ����Ϣ\n====================\n��߷֣�%4d ��ͷ�%4d ƽ���֣�%4f\n", Max, Min, ave);
	return 0;
}
long AutoCountable::IdIterator = 0L;
int main()
{
	char input;
	load();
StartMenu:
	printf("ѧ���ɼ�����ϵͳ\n�������������ִ������\n1.¼��/�޸ĳɼ�\n2.��ѯ�ɼ�\nq.�˳�ϵͳ\n>>> ");
	while (input = getchar())
	{
		switch (input)
		{
		case '1':
			goto InputScoreMenu;
		case '2':
			goto QueryScoreMenu;
		case 'q':
			goto Quit;
		}
	}

InputScoreMenu:
	printf("1.����ѧ����Ϣ\n2.����γ���Ϣ\n3.¼��ɼ�\n4.ɾ��ѧ����Ϣ\n5.ɾ���γ���Ϣ\n6.ɾ��ѧ��-�γ̳ɼ�\nq.����\n>>> ");
	while (input = getchar())
	{
		switch (input)
		{
		case '1':
			importStudentInfo();
			goto InputScoreMenu;
		case '2':
			importCourseInfo();
			goto InputScoreMenu;
		case '3':
			importCourseAndScore();
			goto InputScoreMenu;
		case '4':
			deleteStudent();
			goto InputScoreMenu;
		case '5':
			deleteCourse();
			goto InputScoreMenu;
		case '6':
			deleteScore();
			goto InputScoreMenu;
		case 'q':
			goto StartMenu;
		default:
			continue;
		}
	}
QueryScoreMenu:
	printf("1.��ѯѧ�����п�Ŀ�ɼ�\n2.��ѯһ�ſ�Ŀ����ѧ���ɼ�\nq.����\n>>> ");
	while (input = getchar())
	{
		switch (input)
		{
		case '1':
			QueryStudent();
			goto QueryScoreMenu;
		case '2':
			QueryCourse();
			goto QueryScoreMenu;
		case 'q':
			goto StartMenu;
		}
	}
Quit:
	save();
	return 0;
}
