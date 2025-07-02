#include "student.h"

Student::Student(std::string name, std::string id){
    this->student_id = id;
    this->student_name = name;
}

void Student::addCourse(int credits, std::string name, std::string id){
    courses.push_back(Course(credits,name, id));    
}

std::string Student::getName() const{
    return student_name;
}

std::string Student::getId() const{
    return student_id;
}

std::vector<Course> Student::getCourses() const{
    return courses;
}
