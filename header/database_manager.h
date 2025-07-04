#ifndef DB_MANAGER_H
#define DB_MANAGER_H
#include <sqlite3.h>
#include "student.h"
#include "course.h"
#include <string.h>
#include <vector>

class DB_Manager{
public:
	sqlite3* db;
	int rc; 
	DB_Manager();
	~DB_Manager();
	std::vector<Student> getStudents();
	static int call_back_get_student(void* data, int argc, char** argv, char** azColName);
	void getCourses(std::vector<Student>* students);
	std::vector<Course> getCourses();
	void insertStudent(std::vector<Student> students);
	void deleteStudent(std::string student_id);

	

};

#endif



