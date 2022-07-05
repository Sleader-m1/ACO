#include"ant.h"
#include"Rondomizer.h"
#include<cmath>
#include<algorithm>
#include<iostream>

Ants::Ants(int count_of_ants, int importance_of_pheromone, int importance_of_distance, double evaporation_of_pheromone, double pheromones_from_ant) {
	ants_count = count_of_ants;  //������� �������� ���������� ��������
	pheromone_importance = importance_of_pheromone; //������� �������� �������� ��������
	distance_importance = importance_of_distance; //������� �������� �������� ���������
	pheromones_evaporation = evaporation_of_pheromone; //������� �������� ������������ ���������
	pheromone_from_ant = pheromones_evaporation; //������� �������� ���������� ����������� ����� �������� �������� �� ���� ������
	vertex_count = 0;
	indexes_of_traveled_edges.resize(ants_count);
	best_length = 1000000;
}

bool Ants::InitGraph(const std::string filename, int beginning_vertex) {
	bool result = graph.GetGraphFromFile(filename); //�������� ������� ������ ����� �� ����� � ���������� �� ��� ������� ������� GetGraphFromFile
	vertex_count = graph.GetSize();//������� �������� ���������� ������
	for (int k = 0; k < ants_count; ++k) {
		indexes_of_traveled_edges[k].push_back(beginning_vertex);
	}
	return result;//���������� ����� ������� getGraphFromFile
}

bool Ants::AntWasInVertex(int ant_num, int vertex_num) {
	return find(indexes_of_traveled_edges[ant_num].begin(), indexes_of_traveled_edges[ant_num].end(), vertex_num)
		!= indexes_of_traveled_edges[ant_num].end();//���� ������� ���� �������� ��������, �� ���������� true, ����� - false
}

double Ants::ProbabilityOfVertex(int from_city, int to_city, int ant_num) {
	auto matrix = graph.GetMatrix();//�������� ����� ������� �����

	double Tij = std::pow(matrix[from_city][to_city].second, pheromone_importance);//��������� ���������� �������� �� ����, ����������� � �������
	double Nij = std::pow(1 / matrix[from_city][to_city].first, distance_importance);//��������� ��������, �������� ����������, ����������� � �������

	double current_TijNij = Tij * Nij;//���������� "�����������������" ������� �������
	double sum = Tij * Nij;//����������, �������� "�����������������" ���� ������

	if (matrix[from_city][to_city].first > 0) {
		for (int i = 0; i < vertex_count; ++i) {
			if (i != to_city && matrix[from_city][i].first > 0)
				if (!AntWasInVertex(ant_num, i)) {
					Tij = std::pow(matrix[from_city][to_city].second, pheromone_importance);
					Nij = std::pow(1 / matrix[from_city][to_city].first, distance_importance);
					sum += Tij * Nij; //���������� "�����������������" i-� �������
				}
		}
	}
	else return 0;

	return current_TijNij / sum;//��������� ����������� ����, ��� ������� ������� ����� "���������������" ������
}

int Ants::ChooseNextVertex(int ant_num) {
	int last_vertex_index = indexes_of_traveled_edges[ant_num][indexes_of_traveled_edges[ant_num].size() - 1];//�������� ������ ���������� ������, � ������� ������� ���

	auto prob_rand = GetProbabilityRand();//�������� ��������� ������� ����� �� 0 �� 1
	int i = -1; //�������
	double prob_sum = 0; //���������� �������� ����� ������������
	while (prob_sum < prob_rand) {//�� ��� ��� ���� ���� ��������� ����� ������ ����� ������������ ������� ����� �������������
		++i;
		if (!AntWasInVertex(ant_num, i))
			prob_sum += ProbabilityOfVertex(last_vertex_index, i, ant_num);//���������� � ����� ����������� �������� � i-�� �������
	}
	return i;//���������� �������, ������� �������� ����� ������, ������� ����� ��������� �������
}

int Ants::HaveValidVertex(int ant_num) {
	auto matrix = graph.GetMatrix();

	int last_visited_city = indexes_of_traveled_edges[ant_num][indexes_of_traveled_edges[ant_num].size() - 1];

	for (int i = 0; i < vertex_count; ++i) {
		if (matrix[last_visited_city][i].first > 0) {
			if (!AntWasInVertex(ant_num, i)) {
				return i;
			}
		}
	}

	return -1;
}

void Ants::MakeIteratoin(int iteration_count) {
	for (int iteration = 0; iteration < iteration_count; ++iteration) {
		for (int k = 0; k < ants_count; ++k) {
			while (HaveValidVertex(k) != -1) {
				auto v = HaveValidVertex(k);
				indexes_of_traveled_edges[k].push_back(ChooseNextVertex(k));//������ � ������ ������ �������, � ������� ������� ������ �� ���� ����
			}
		}//������ ��� ��� ���� �������� � ������� �����

		for (int i = 0; i < vertex_count; ++i) {
			for (int j = 0; j < vertex_count; ++j) {
				if (i != j) {
					graph.graph_matrix[i][j].second *= (1 - pheromones_evaporation);//�������� �� ���� ������� ����� ������������ ��������� � �������� ���������� ��������
				}
			}
		}

		//����� �� ����� ���������� �������� � ��������������� �����
		//� ������ ���������� ������ �������� ���� �������, ������� ���������� � ��������� �������� ���������� ������ � ���� ����� �������
		for (int k = 0; k < ants_count; ++k) {
			if (indexes_of_traveled_edges[k].size() < vertex_count)
				continue;
			int index_of_first_visited_city = indexes_of_traveled_edges[k][0];
			int index_of_last_visited_city = indexes_of_traveled_edges[k][indexes_of_traveled_edges[k].size() - 1];

			double length = 0;

			for (int i = 0; i < indexes_of_traveled_edges[k].size() - 1; ++i) {

				int index_of_fromcity = indexes_of_traveled_edges[k][i];//���������� �����, � ������� ��� �������
				int index_of_tocity = indexes_of_traveled_edges[k][i + 1];//�����, � ������� ������� ���������� �� ����������� ������

				double distance_between_cities = graph.graph_matrix[index_of_fromcity][index_of_tocity].first;//���������� ����� ����� ��������

				length += distance_between_cities;

				graph.graph_matrix[index_of_fromcity][index_of_tocity].second += pheromone_from_ant / distance_between_cities;//��������� ������� �� ������� �� ������
				graph.graph_matrix[index_of_tocity][index_of_fromcity].second += pheromone_from_ant / distance_between_cities;//��������� � ������������ ������ �������
			}

			length += graph.graph_matrix[index_of_first_visited_city][index_of_last_visited_city].first;

			if (length < best_length) {
				best_length = length;
				best_route = indexes_of_traveled_edges[k];
			}

			indexes_of_traveled_edges[k].resize(1); //��������� ���������� ���� ��� k-�� �������, �������� ������ ��������� �����
		}

		std::cout << "On iteration " << iteration + 1 << " the best length is " << best_length << std::endl;
	}
	std::cout << "Best route: ";
	for (auto vertex : best_route) {
		std::cout << vertex << " ";
	}
	std::cout << std::endl;
}

void Ants::PrintMatrix() {
	auto matrix = graph.GetMatrix();

	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix.size(); ++j) {
			std::cout << matrix[i][j].first << " ";
		}
		std::cout << std::endl;
	}
}