#include "database_manager.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>


DB_Manager::DB_Manager(){
	//Initialize DB
	this->rc = sqlite3_open("/home/ram/Sriram_Tatikonda/Sriram2/VS code projects/C++/student_management_system/database/test.db", &db);
	if (rc) {
		std::cout << "ERR: Could not connect to database\n";
	}

}

DB_Manager::~DB_Manager(){
	sqlite3_close(db);

}

std::vector<Student> DB_Manager::getStudents(){
	char* err_message;

	std::vector<Student> students;

	std::string query = "select * from students";
	
	
	this->rc = sqlite3_exec(db,query.c_str(), DB_Manager::call_back_get_student, &students, &err_message);

	if (this->rc != SQLITE_OK){
		std::cout << err_message << std::endl;
	}
	else{
		std::cout << "Students data successfully retrieved" << std::endl;
	}

	for (auto i:students){
		std::cout << i.getId() << "\n";
	}

	DB_Manager::getCourses(&students);

	for (auto student:students){
		std::vector<Course> courses = student.getCourses();
		for (auto course:courses){
      			std::cout << course.getCourseId() << "\n";
		}
	}
	
	return students;
}
 
int DB_Manager::call_back_get_student(void* data, int argc, char** argv, char** azColName){
	std::vector<Student>* students = static_cast<std::vector<Student>*>(data);
	students->push_back(Student(argv[1],argv[0]));
   	return 0;
}

void DB_Manager::getCourses(std::vector<Student>* students){
	sqlite3_stmt* stmt;

	const char* query = "select courses.course_id, courses.course_name, courses.course_credits from courses join registrations on registrations.course_id = courses.course_id where student_id = ?;";

	if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK){
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
		return;
	}

	for (auto& student:*students){
		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
		std::string std_id = student.getId();
		sqlite3_bind_text(stmt, 1, std_id.c_str(), -1, SQLITE_TRANSIENT);

		while (sqlite3_step(stmt) == SQLITE_ROW){
			std::string course_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			std::string course_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
			int course_credits = sqlite3_column_int(stmt,2);
			student.addCourse(course_credits,course_name,course_id);
		}
	
	}


	
}

void DB_Manager::insertStudent(const Student& student){
	// Use DB to insert student in student table
	char* err_message;
	std::string sql_query = "insert into students(student_id, student_name) values (" + std::string("'") + student.getId()+ "'"+ ","+ "'" + student.getName()+"');";
	this->rc = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &err_message);	
	if (rc != SQLITE_OK){
		std::cout << "ERR: " << err_message << "\n";
	}
	else {
		std::cout << "Inserted Student Successfully\n";
	}
	sqlite3_free(err_message);

}

void DB_Manager::insertCourse(const Course& course){
	// Use DB to insert course in course table
	char* err_message;
	std::string sql_query = "insert into courses(course_id, course_name, course_credits) values ('" + course.getCourseId() + "'" + "," + "'" + course.getCourseName() + "'" + ","  + std::to_string(course.getCredits()) + ");";

	this->rc = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &err_message);
	if (rc != SQLITE_OK){
		std::cout << "ERR: " << err_message << "\n";
	}
	else{
		std::cout << "Inserted Course Successfully\n";
	}
	sqlite3_free(err_message);


	 
}

void DB_Manager::addStudentCourse(const Student& student){
	// Use DB to add course and student details to course and student relation table
	
	char* err_message;
	std:: string sql_query;

	for (auto course: student.getCourses()){
		sql_query = "insert into registrations(student_id, course_id) values ('" + student.getId() + "'" + "," + "'" + course.getCourseId() + "');";
		rc = sqlite3_exec(db, sql_query.c_str(), nullptr, nullptr, &err_message);

		if (rc != SQLITE_OK){
			std::cout << "ERR: " << err_message << "\n";
		}
		else{
			std::cout << "Added student and course relation\n";
		}
		sqlite3_free(err_message);

	}
}
