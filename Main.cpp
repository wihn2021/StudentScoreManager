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
	printf("请输入文件名称。文件内格式应为：学号+空格+姓名\n>>> ");
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
		printf("导入错误\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
		return 1;
	}
	printf("学生信息导入成功\n");
	return 0;
}
int importCourseInfo()
{
	printf("请输入文件名。文件内一行一个课程名\n>>> ");
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
		printf("导入错误\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
	}
	printf("课程信息导入成功\n");
	return 0;
}
int importCourseAndScore()
{
	printf("请输入文件名。文件内应为：学号+空格+课程名+空格+成绩\n>>> ");
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
		printf("导入错误\n");
		return 1;
	}
	catch (const char* c)
	{
		printf("%s\n", c);
	}
	printf("成绩导入成功\n");
	return 0;
}
int deleteStudent()
{
	printf("请输入学生学号来删除所有相关的成绩（无法撤销）\n>>> ");
	long long stuid;
	std::cin >> stuid;
	auto t = ssm.FindStudentPtrByStuid(stuid);
	if (t != nullptr)
	{ 
		ssm.deleteStudent(t);
		printf("删除成功\n");
		return 0;
	}
	else printf("StudentNotExist\n");
	return 1;
}
int deleteCourse()
{
	printf("请输入课程名称来删除所有相关的信息（无法撤销）\n>>> ");
	std::string name;
	std::cin >> name;
	auto t = ssm.FindCoursePtrByName(name);
	if (t != nullptr)
	{
		ssm.deleteCourse(t);
		printf("删除成功\n");
		return 0;
	}
	else printf("CourseNotExist\n");
	return 1;
}
int deleteScore()
{
	printf("请输入学生学号+空格+课程名称\n>>> ");
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
	printf("删除成功\n");
	return 0;
}
int QueryStudent()
{
	printf("请输入学生学号：\n>>> ");
	long long s;
	try
	{
		std::cin >> s;
		auto p = ssm.FindStudentPtrByStuid(s);
		if (p == nullptr)
		{
			throw "StudentNotExist";
		}
		printf("姓名：%s\n学号：%lld\n", p->name.c_str(), p->stuid);
		for (auto i : p->scores)
		{
			printf("%10s%4d\n", ((Score*)i)->co->name.c_str(), ((Score*)i)->getValue());
		}
	}
	catch (std::exception& e)
	{
		printf("输入学号有误！\n");
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
	printf("请输入课程名称\n>>> ");
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
	printf("%10s %5s %4s\n", "学号", "姓名", "成绩");
	for (auto i : c->scores)
	{
		printf("%10lld %5s %4d\n", ((Score*)i)->stu->stuid, ((Score*)i)->stu->name.c_str(), i->getValue());
		if (i->getValue() > Max) Max = i->getValue();
		if (i->getValue() < Min) Min = i->getValue();
		Sum += i->getValue();
	}
	ave = Sum / c->scores.size();
	printf("====================\n统计信息\n====================\n最高分：%4d 最低分%4d 平均分：%4f\n", Max, Min, ave);
	return 0;
}
long AutoCountable::IdIterator = 0L;
int main()
{
	char input;
	load();
StartMenu:
	printf("学生成绩管理系统\n输入以下序号以执行命令\n1.录入/修改成绩\n2.查询成绩\nq.退出系统\n>>> ");
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
	printf("1.导入学生信息\n2.导入课程信息\n3.录入成绩\n4.删除学生信息\n5.删除课程信息\n6.删除学生-课程成绩\nq.返回\n>>> ");
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
	printf("1.查询学生所有科目成绩\n2.查询一门科目所有学生成绩\nq.返回\n>>> ");
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
