//���� ����������� ������� ������ Graph
#include"Graph.h"
#include<fstream>

Graph::Graph(std::vector<std::vector<edge>> matrix) {
	graph_matrix = matrix;//������� �����������, ������� �������� � ���� ��������� ������� 
}

bool Graph::GetGraphFromFile(const std::string filename) {
	std::ifstream file(filename); //��������� ����
	
	if (!file) return false;//��������� �������� �� ����, ���������� �������� false, ���� ���� �� �������� 

	int vertex_count; //����������, � ������� ����� ������� ���������� ������
	file >> vertex_count; //�������� �������� ���������� ������
	for (int i = 0; i < vertex_count; ++i) {
		std::vector<edge>new_row; //������� ��������������� ����������, �������� � ���� ������� �������� ��� ��������
		for (int j = 0; j < vertex_count; ++j) { 
			double new_distance; //�������� ����� �����
			file >> new_distance;
			new_row.push_back(std::make_pair(new_distance, 0.1));//��������� � ��� �������� ���������, ��� ���� ����������� �������� ��������� � 0.1
		}
		graph_matrix.push_back(new_row); //��������� ��������������� ��� � �������� �������
	}

	file.close();//��������� ����
	return true;//���������� �������� true, ���� ��������, ��� ������ ������ �������
}

std::vector<std::vector<edge>> Graph::GetMatrix() const{
	return graph_matrix;//������� �������-������, ������� ����� ����������� const, �� �� �� ������ ������� ��������
}

int Graph::GetSize() {
	return graph_matrix.size();
}

std::vector<std::vector<edge>>& Graph::GetOriginalMatrix() {
	return graph_matrix;
}