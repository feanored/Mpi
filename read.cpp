#include <iostream>
#include <fstream>
#include <iomanip>

struct Log {
	int f;
	double x;
	double y;
};

int main(int argc, char* argv[]) {
	std::ifstream inFile("log.bin", std::ios::out | std::ios::binary);
	if (!inFile) {
		std::cout << "Cannot open file!" << std::endl;
		return 1;
	}

	Log value{};
	inFile.read( (char *) &value, sizeof(Log));
	inFile.close();

	if (!inFile.good()) {
		std::cout << "Error occurred at reading time!" << std::endl;
		return 1;
	}

	std::string function;
	switch (value.f) {
	case 1:
		function = "addition"; break;
	case 2:
		function = "multiplication"; break;
	case 3:
		function = "atan"; break;
	case 4:
		function = "acos"; break;
	case 5:
		function = "4*atan"; break;
	}

	std::cout << "Log file contains: " << std::endl;
	std::cout << function << "(";
	std::cout << std::setprecision(15) << value.x << ") = ";
	std::cout << std::setprecision(15) << value.y << std::endl;
	return 0;
}