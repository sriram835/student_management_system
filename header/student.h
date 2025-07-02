#ifndef STUDENT_H
#define STUDENT_H
#include <course.h>
#include <string>
#include <vector>

class Student {
private:
    std::string student_name;
    std::string student_id;
    std::vector<Course> courses;

public:
    Student(std::string name, std::string id);
    void addCourse(int credits, std::string name, std::string id);
    std::string getName() const;
    std::vector<Course> getCourses() const;
    std::string getId() const;
};

#endif
