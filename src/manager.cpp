#include "manager.h"
#include <limits>
#include "database_manager.h"

std::vector<Student> Manager::students;
std::vector<Course> Manager::courses;
DB_Manager Manager::databaseManager;

void Manager::execute(){
    std::string choices_details = "1. Add student\n2. Add a course \n3. Register Student to a course\n4. Display the student details\n5. Delete Student\n6. Exit\n\nEnter your Choice: ";
    int choice;
	

	Manager::students = databaseManager.getStudents();
	Manager::courses = databaseManager.getCourses();

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
		registerStudentToCourse();
		break;
        case 4:
            displayDetails();
            break;
            
	case 5:
		deleteStudent();
		break;
        case 6:
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
	databaseManager.insertStudent(students);

        std::cout << "\nStudent added!\n" << std::endl;

}

void Manager::addCourse(){


    std::string course_name;
    int course_credits;
	std::string course_id;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "Enter Course ID: ";
	getline(std::cin, course_id);
	std::cout << std::endl;

    std::cout << "Enter Course Name: ";
    getline(std::cin, course_name);
    std::cout << std::endl;

    std::cout << "Enter Credits: ";
    std::cin >> course_credits;
    std::cout << std::endl;
    
	courses.push_back(Course(course_credits, course_name, course_id));
	databaseManager.insertCourse(courses);
    std::cout << "Course Added!" << std::endl;
}

void Manager::registerStudentToCourse() {
	int count = 0;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (Student student:students){
		std::cout << "-----------------------------\n";
		std::cout << "Index: " << count++ << "\n";
		std::cout << "Student Name: " << student.getName() << "\n";
		std::cout << "Student Id: " << student.getId() << "\n";
		std::cout << "-----------------------------\n\n";
	}

	int std_index;

	std::cout << "Enter the index of student: ";
	std::cin >> std_index;
	std::cout << "\n";

	count = 0;
	for (Course course:courses){
		std::cout << "-----------------------------\n";
		std::cout << "Index: " << count++ << "\n";
		std::cout << "Course Name: " << course.getCourseName() << "\n";
		std::cout << "Course ID: " << course.getCourseId() << "\n";
		std::cout << "Course Credits: " << course.getCredits() << "\n";
		std::cout << "------------------------------\n\n";
	}

	int course_index;

	std::cout << "Enter the index of Course: ";
	std::cin >> course_index;
	std::cout << "\n";

	int result = databaseManager.addCourseToStudent(students[std_index].getId(), courses[course_index].getCourseId());

	if (result != SQLITE_DONE){
		return;
	}


	students[std_index].addCourse(courses[course_index].getCourseId());
	std::cout << "Successfully registered Student to course\n";

}

void Manager::displayDetails(){
    for (Student student:students){
        std::cout << "------------------------------" << std::endl;
        std::cout << "Student Name: " << student.getName() << std::endl;
        std::cout << "Student ID: " << student.getId() << std::endl;
        std::cout << "Number of Courses enrolled: " << student.getCourses().size() << std::endl;

		//Write the function to display  courses.
        std::cout << "------------------------------" << std::endl;
    }

	std::cout << "\n";
}


void Manager::deleteStudent(){
    int count = 0;
    int index;

    for (Student student:students){
        std::cout <<  "--------------------------------\n";
	std::cout << "Index: " << count++ << "\n";
	std::cout << "Student Name: " << student.getName() << std::endl;
        std::cout << "Student ID: " << student.getId() << "\n--------------------------------\n";
    }

    std::cout << "\nEnter the index of the student you want to delete: ";
    std::cin >> index;
    std::cout << std::endl;

	std::string student_id = students[index].getId();
	databaseManager.deleteStudent(student_id);
	students.erase(students.begin() + index);

}


Manager::~Manager(){
	databaseManager.insertStudent(students);
	databaseManager.insertCourse(courses);
}



