#include <sqlite3.h>
#include <string>
#include <iostream>

int main(){
	std::string query = "insert into registrations(student_id,course_id) values (?,?);";
	std::string err_msg;

	std::string std_id = "cse1";
	std::string course_id = "CSE";

	sqlite3* db;
	int rc;
	rc = sqlite3_open("/home/ram/Sriram_Tatikonda/Sriram2/VS code projects/C++/student_management_system/database/test.db", &db);
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

	if (rc) {
		std::cout << "ERR: Could not connect to database\n";
	}
	
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db,query.c_str(),-1, &stmt, nullptr) != SQLITE_OK){
		std::cout << "Err when preparing stmt\n";
	}

	sqlite3_bind_text(stmt,1,std_id.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt,2,course_id.c_str(), -1, SQLITE_TRANSIENT);

	rc = sqlite3_step(stmt);
	if (rc = SQLITE_CONSTRAINT){
		std::cout << "ERR: " << sqlite3_errmsg(db) << "\n";
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
