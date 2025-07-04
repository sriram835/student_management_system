#include "student.h"

Student::Student(std::string name, std::string id){
    this->student_id = id;
    this->student_name = name;
}

void Student::addCourse(std::string course_id){
    course_ids.push_back(course_id);    
}

std::string Student::getName() const{
    return student_name;
}

std::string Student::getId() const{
    return student_id;
}

std::vector<std::string> Student::getCourses() const{
    return course_ids;
}
