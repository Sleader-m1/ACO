#include <iostream>
#include"ant.h"

int main() {
	std::string filename = "matrix.txt";
	Ants* ants = new Ants(40, 1, 1, 0.1, 0.02);
	std::cout << ants->InitGraph(filename, 5);
	ants->PrintMatrix();
	ants->MakeIteratoin(25);
}