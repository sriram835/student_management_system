#include "manager.h"
#include <limits>

std::vector<Student> Manager::students;

void Manager::execute(){
    std::string choices_details = "1. Add student\n2. Add a course to student\n3. Display the student details\n4. Exit\nEnter your Choice: ";
    int choice;

    while (1){
        std::cout << choices_details;
        std::cin >> choice;
        std::cout << std::endl;
        
        switch (choice)
        {
        case 1:
            addStudent();
            break;
        
        case 2:
            addCourse();
            break;

        case 3:
            displayDetails();
            break;
            
        case 4:
            return;

        default:
        std::cout << "Invalid" << std::endl;
        break;
        }
    }
}

void Manager::addStudent(){
        std::string name;
        std::string id;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter the student name: ";
        getline(std::cin, name);
        std::cout << std::endl;

        std::cout << "Enter the student id: ";
        getline(std::cin, id);
        std::cout << std::endl;
        
        students.push_back(Student(name,id));
        std::cout << "\n\nStudent added!\n" << std::endl;
}

void Manager::addCourse(){
    int count = 0;
    int index;

    for (Student student:students){
        std::cout << count++ << "-------------------------------\n" << "Student Name: " << student.getName() << std::endl;
        std::cout << "Student ID: " << student.getId() << "--------------------------------" << std::endl;
    }

    std::cout << "Enter the index of the student: ";
    std::cin >> index;
    std::cout << std::endl;


    std::string course_name;
    int course_credits;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Course Name: ";
    getline(std::cin, course_name);
    std::cout << std::endl;

    std::cout << "Enter Credits: ";
    std::cin >> course_credits;
    std::cout << std::endl;
    
    students[index].addCourse(course_credits, course_name);
    std::cout << "Course Added!" << std::endl;
}

void Manager::displayDetails(){
    for (Student student:students){
        std::cout << "------------------------------" << std::endl;
        std::cout << "Student Name: " << student.getName() << std::endl;
        std::cout << "Student ID: " << student.getId() << std::endl;
        std::cout << "Number of Courses enrolled: " << student.getCourses().size() << std::endl;

        for (Course course:student.getCourses()){
            std::cout << "\t--------------------------" << std::endl;
            std::cout << "\tCourse Name: " << course.getCourseName() << std::endl;
            std::cout << "\tCourse Credits: " << course.getCredits() << std::endl;
            std::cout << "\t--------------------------" << std::endl;
        }
        
        std::cout << "------------------------------" << std::endl;
    }
}