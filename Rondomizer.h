#pragma once

#include<cstdlib>

double GetProbabilityRand() { //������� ��������� ���������� ����� � ��������� �� 0 �� 1
	return (double)rand() / (double)RAND_MAX ;
}