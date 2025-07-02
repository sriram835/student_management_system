#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course{
private:
	int credits;
	std::string course_name;
	std::string course_id;

public:
	Course(int credits, std::string course_name, std::string course_id);
    	int getCredits() const;
	std::string getCourseName() const;
	std::string getCourseId() const;
};

#endif
