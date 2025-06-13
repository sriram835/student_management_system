#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course{
private:
    int credits;
    std::string course_name;

public:
    Course(int credits, std::string course_name);
    int getCredits();
    std::string getCourseName();

};

#endif