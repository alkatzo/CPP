// Task1.cpp : Defines the entry point for the application.
//

#include "Task1.h"

#include <format>

using namespace std;

int main()
{
	std::string name;
	int age;

	cout << "Enter your name: ";
	cin >> name;
	cout << "Enter your age: ";
	cin >> age;

	std::string message = std::format("Hello {}, you are {} years old.", name, age);
	cout << message << endl;

	return 0;
}
