#pragma once

#include"Graph.h"

class Ants { //����� �������
	Graph graph;

	int pheromone_importance,
		distance_importance; //�������� ���������� ���� � ����� �� ������� ����������� ��������������
	double pheromones_evaporation,//����������� ��������� ��������
		pheromone_from_ant; // �������, ����������� �������� �� ��� ������

	int ants_count, //���������� �������� � ��������
		vertex_count;//���������� ������ �����

	double best_length; //���������� ����� ����������� ����

	std::vector<std::vector<int>> indexes_of_traveled_edges; //������ ������� ���������� ������
	std::vector<int> best_route;
public:
	Ants(int count_of_ants, int importance_of_pheromone, int importance_of_distance, double evaporation_of_pheromone, double pheromones_from_ant);
	int ChooseNextVertex(int ant_num); // �������, ���������� ��� ������� ��������� �������
	void MakeIteratoin(int iteration_count);//�������, ����������� ��������� ���������� ���������� ��������
	double ProbabilityOfVertex(int from_city, int to_city, int ant_num);//�������, ������������ ����������� ����, ��� ������� ������ �� ���� from_city-to_city
	bool InitGraph(const std::string filename, int beginning_vertex);//������� ������ ��� �����, �������� �������� ��� ������� ������ ����� �� �����
	bool AntWasInVertex(int ant_num, int vertex_num);//�������, ������������ true, ���� ������� �������� �� ��������� ��������, false-� ��������� ������
	int	HaveValidVertex(int ant_num);//�������, ������������ true, ���� ������� �� ����� � "�����", � ��������� ������-false
	void PrintMatrix();
};

