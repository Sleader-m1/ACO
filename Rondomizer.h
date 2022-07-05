#pragma once

#include<cstdlib>

double GetProbabilityRand() { //функция генерации случайного числа в диапазоне от 0 до 1
	return (double)rand() / (double)RAND_MAX ;
}