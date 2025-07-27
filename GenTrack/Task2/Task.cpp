// Task1.cpp : Defines the entry point for the application.
//

#include "Task.h"

#include <vector>
#include <iostream>
#include <sstream>



using namespace std;

int main()
{
	std::vector<int> numbers;
	std::string line;

	std::cout << "Enter a list of integers (space-separated): ";
	std::getline(std::cin, line); // read the whole line

	std::istringstream iss(line);
	int num;

	while (iss >> num) {
		numbers.push_back(num);
	}

	// Output to verify
	std::cout << "You entered: ";
	for (int n : numbers) {
		std::cout << n << " ";
	}
	std::cout << "\n";


	return 0;
}
