#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <vector>
#include <student.h>
#include <iostream>
class Manager{

public:
    static std::vector<Student> students;
    void execute();
    static void addStudent();
    static void addCourse();
    static void displayDetails();
};

#endif