#include "student.h"

Student::Student(std::string name, std::string id){
    this->student_id = id;
    this->student_name = name;
}

void Student::addCourse(int credits, std::string name){
    courses.push_back(Course(credits,name));    
}

std::string Student::getName(){
    return student_name;
}

std::string Student::getId(){
    return student_id;
}

std::vector<Course> Student::getCourses(){
    return courses;
}