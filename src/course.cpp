
#include "course.h"
Course::Course(int credits, std::string course_name){
    this->course_name = course_name;
    this->credits = credits;
}

int Course::getCredits(){
    return credits;
}

std::string Course::getCourseName(){
    return course_name;
}
