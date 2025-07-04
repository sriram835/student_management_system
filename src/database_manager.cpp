#include "database_manager.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>


DB_Manager::DB_Manager(){
	//Initialize DB
	this->rc = sqlite3_open("/home/ram/Sriram_Tatikonda/Sriram2/VS code projects/C++/student_management_system/database/test.db", &db);
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

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
		std::cout << "\nStudents data successfully retrieved" << "\n\n";
	}


	DB_Manager::getCourses(&students);

	
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

	sqlite3_finalize(stmt);
	
}

void DB_Manager::insertStudent(const std::vector<Student> students){
	// Use DB to insert student in student table
	char* err_message;

	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_message);

	sqlite3_stmt* std_stmt;
	if (sqlite3_prepare_v2(db, "insert or replace into students(student_id,student_name) values(?,?);", -1, &std_stmt, nullptr) != SQLITE_OK){
		std::cerr << "Failded to prepare student stmt during insert: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(std_stmt);
		return;
	}

	sqlite3_stmt* course_stmt;
	if (sqlite3_prepare_v2(db, "insert or replace into courses(course_id,course_name,course_credits) values (?,?,?);", -1, &course_stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failded to prepare course stmt during insert: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(course_stmt);
		return;
	}

	sqlite3_stmt* register_stmt;
	if (sqlite3_prepare_v2(db, "insert or ignore into registrations(student_id,course_id) values (?,?);", -1, &register_stmt,nullptr) != SQLITE_OK){
		std::cerr << "Failded to prepare registrations stmt during insert: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(register_stmt);
		return;
	}
	
	for (const auto student:students){
		
		sqlite3_bind_text(std_stmt,1, student.getId().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(std_stmt,2,student.getName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(std_stmt);
		sqlite3_reset(std_stmt);

		for (const auto course:student.getCourses()){
			sqlite3_bind_text(course_stmt, 1, course.getCourseId().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(course_stmt, 2, course.getCourseName().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(course_stmt, 3, course.getCredits());

			sqlite3_step(course_stmt);
			sqlite3_reset(course_stmt);


			sqlite3_bind_text(register_stmt, 1, student.getId().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(register_stmt, 2, course.getCourseId().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_step(register_stmt);
			sqlite3_reset(register_stmt);
		}
	}


	sqlite3_finalize(std_stmt);
	sqlite3_finalize(course_stmt);
	sqlite3_finalize(register_stmt);
	sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_message);
}

void DB_Manager::deleteStudent(std::string student_id){

	std::string query = "delete from students where student_id = '" + student_id + "';";

	char* err_message;

	if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_message) != SQLITE_OK){
		std::cerr << "ERR when deleting student: " << err_message << "\n";
	}
	else {
		std::cout << "Student is successfully deleted\n";
	}
	
}
