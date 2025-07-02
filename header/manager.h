#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <vector>
#include "student.h"
#include <iostream>
#include "database_manager.h"

class Manager{

public:
    static std::vector<Student> students;
	static DB_Manager databaseManager; 
    void execute();
    static void addStudent();
    static void addCourse();
    static void displayDetails();
};

#endif
