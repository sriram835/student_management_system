#include <chrono>
#include <string>
#include <iostream>
using namespace std;
using namespace std::chrono;

class MyObj {
public:
    string id;
    string name;

    MyObj(string id, string name) {
        this->id = id;
        this->name = name;
    }

    // Overload == operator
    bool operator==(const MyObj& other) const {
        return id == other.id ;
    }
};

bool compare_obj(const MyObj& obj1, const MyObj& obj2) {
    return obj1 == obj2;
}

bool compare_string(const string& str1, const string& str2) {
    return str1 == str2;
}

int main(){
	MyObj obj1("cse1", "ram");
	MyObj obj2("cse2", "sri");
	string str1 = "cse1";
	string str2 = "cse2";
	auto start_string = high_resolution_clock::now();
	compare_string( str1,  str2);
	auto end_string = high_resolution_clock::now();

	auto start_obj = high_resolution_clock::now();
	compare_obj( obj1,  obj2);
	auto end_obj = high_resolution_clock::now();

	auto duration_string = chrono::duration_cast<nanoseconds>(end_string - start_string);
	auto duration_obj = chrono::duration_cast<nanoseconds>(end_obj - start_obj);

	std::cout << "String time: " << duration_string.count() << "\n";
	std::cout << "Object time: " << duration_obj.count() << "\n";
	
}




