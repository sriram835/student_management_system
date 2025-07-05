#include "database_manager.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>


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

	
	return students;
}
 
int DB_Manager::call_back_get_student(void* data, int argc, char** argv, char** azColName){
	std::vector<Student>* students = static_cast<std::vector<Student>*>(data);
	students->push_back(Student(argv[1],argv[0]));
   	return 0;
}

std::vector<Course> DB_Manager::getCourses(){
	sqlite3_stmt* stmt;
	std::vector<Course> courses;

	const char* query = "select course_id, course_name, course_credits from courses;";

	if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK){
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
		return courses;
	}


	while (sqlite3_step(stmt) == SQLITE_ROW){
		std::string course_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
		std::string course_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
		int course_credits = sqlite3_column_int(stmt,2);
		courses.push_back(Course(course_credits,course_name,course_id));
	}

	sqlite3_finalize(stmt);

	return courses;
}

void DB_Manager::insertStudent(const std::vector<Student> students){
	// Use DB to insert student in student table
	char* err_message;

	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_message);

	sqlite3_stmt* std_stmt;
	if (sqlite3_prepare_v2(db, "insert into students(student_id,student_name) values(?,?) on conflict(student_id) do update set student_name = excluded.student_name;", -1, &std_stmt, nullptr) != SQLITE_OK){
		std::cerr << "Failded to prepare student stmt during insert: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(std_stmt);
		return;
	}



	for (const auto student:students){
		
		sqlite3_bind_text(std_stmt,1, student.getId().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(std_stmt,2,student.getName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_step(std_stmt);
		sqlite3_reset(std_stmt);
	}


	sqlite3_finalize(std_stmt);
	sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_message);
}

void DB_Manager::insertCourse(std::vector<Course> courses){
	char* err_message;

	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_message);

	sqlite3_stmt* course_stmt;
	if (sqlite3_prepare_v2(db, "insert into courses(course_id,course_name,course_credits) values(?,?,?) on conflict(course_id) do update set course_name = excluded.course_name, course_credits = excluded.course_credits;", -1, &course_stmt, nullptr) != SQLITE_OK){
		std::cerr << "Failded to prepare student stmt during insert: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(course_stmt);
		return;
	}



	for (const auto course:courses){
		
		sqlite3_bind_text(course_stmt,1, course.getCourseId().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(course_stmt,2, course.getCourseName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(course_stmt, 3, course.getCredits());
		sqlite3_step(course_stmt);
		sqlite3_reset(course_stmt);
	}


	sqlite3_finalize(course_stmt);
	sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_message);


}

int DB_Manager::addCourseToStudent(std::string student_id, std::string course_id){
	std::string query = "insert into registrations(student_id,course_id) values (?,?);";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db,query.c_str(),-1,&stmt,nullptr) != SQLITE_OK){
		std::cout << "Failded to prepare stmt when registering student to course\n";
		return SQLITE_ERROR;
	}

	if (sqlite3_bind_text(stmt,1,student_id.c_str(),-1,SQLITE_TRANSIENT) != SQLITE_OK || sqlite3_bind_text(stmt,2,course_id.c_str(),-1,SQLITE_TRANSIENT) != SQLITE_OK){
		std::cout << "Failed to bind text to stmt when registering student to course: " << sqlite3_errmsg(db) << "\n";
		sqlite3_finalize(stmt);
		return SQLITE_ERROR;
	}

	int result = sqlite3_step(stmt);
	if (result != SQLITE_DONE){
		std::cout << "Failed inserting into registrations: " << sqlite3_errmsg(db) << "\n";
	}

	sqlite3_finalize(stmt);
	return result;

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

void DB_Manager::coupleStudentCourse(std::vector<Student>& students){
	std::unordered_map<std::string,std::vector<std::string>> student_course_map;
	
	std::string err_msg;
	std:: string query = "select student_id, course_id from registrations;";
	
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db,query.c_str(),-1,&stmt,nullptr) != SQLITE_OK){
		std::cout << "Failed preparing stmt to get registrations: " << sqlite3_errmsg(db) << "\n";
		return;
	}

	std::string student_id;
	std::string course_id;

	while (sqlite3_step(stmt) == SQLITE_ROW){
		student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
		course_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
		student_course_map[student_id].push_back(course_id);

	}

	for (Student& student:students){
		student.addCourseVector(student_course_map[student.getId()]);
	}

	sqlite3_finalize(stmt);
}
