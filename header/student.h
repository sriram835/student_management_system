#ifndef STUDENT_H
#define STUDENT_H
#include <course.h>
#include <string>
#include <vector>

class Student {
private:
    std::string student_name;
    std::string student_id;
    std::vector<std::string> course_ids;

public:
    Student(std::string name, std::string id);
    void addCourse(std::string course_id);
	void addCourseVector(std::vector<std::string>& new_course_ids);
    std::string getName() const;
    std::vector<std::string> getCourses() const;
    std::string getId() const;
};

#endif
