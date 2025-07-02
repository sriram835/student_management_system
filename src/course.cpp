
#include "course.h"
Course::Course(int credits, std::string course_name, std::string course_id){
    this->course_name = course_name;
    this->credits = credits;
	this->course_id = course_id;
}

int Course::getCredits() const{
    return credits;
}

std::string Course::getCourseName() const{
    return course_name;
}

std::string Course::getCourseId() const{
	return course_id;
}
