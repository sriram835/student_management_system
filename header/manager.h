#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <vector>
#include "student.h"
#include "course.h"
#include <iostream>
#include "database_manager.h"

class Manager{

public:
    static std::vector<Student> students;
	static std::vector<Course> courses;
	static DB_Manager databaseManager; 
    void execute();
	~Manager();
    static void addStudent();
    static void addCourse();
	static void registerStudentToCourse();
    static void displayDetails();
	static void deleteStudent();
};

#endif
