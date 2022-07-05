#pragma once

//#include<tuple>
//#include<vector>
//#include<iostream>
//
//
//class Vertex {//����� ������� ��� �������� ��������
//	double pos_x, pos_y; //���������� �������
//	bool isEmpty; //������, ������������ ��� �������: ������ ��� ��������
//	short int vertex_id; //��������������� ����� �������
//public:
//	Vertex(double x, double y, short int id): pos_x(x), pos_y(y), isEmpty(true), vertex_id(id){}
//	Vertex(short int id):isEmpty(false), pos_x(-1), pos_y(-1), vertex_id(id) {}
//	std::pair<double, double> GetCoordinates() const; //������-�������, ������� ��������� ��� �������� �����������
//	bool GetTypeOfVertex()const; //������-�������, ������ ��������� � ��� �������� �� ������� ������
//};
//
//class Edge {//����� �����
//	Vertex fvertex, svertex; //�������, ������� ��������� �����
//	double smell; //���������� "���� ������ ���������" �����
//	double distance; //��� �����, ������� ����������� ������� ��� ������ ��� ��������� 
//public:
//	Edge(Vertex first_vertex, Vertex second_vertex):fvertex(first_vertex), svertex(second_vertex), smell(0){}
//	std::pair<Vertex, Vertex> Getvertexes() const; //������-������� ��� ��������� ������ ��� ��������� ��������
//};
//
//class Graph {//����� ����
//protected:
//	std::vector<Edge> edges;
//	std::vector<Vertex> vertexes;
//public:
//	Graph() {};//������ ���������� �� ���������
//	bool GetGraphFromFile(const std::string file_name)const;//������� ������ ����� �� �����, ���������� true, ���� ������ ������ �������
//	std::vector<Vertex> GetNeighbourVertex(const Vertex vertex)const;//�������, ������� ����� ���������� �������� �������� �������� ������� ��� ��������� �������
//};
//
////�������� �������� ���� � ������������� �������, ���� �� ��������� ������� 
////����������� ���������� "����� �� ���� �� ���������?"
//
//class Diagonal_Graph:public Graph{//����� �����, ������������ ������������ �� ���������
//	std::vector<std::vector<Vertex>> graph_matrix; //������� �����, �������� ������� �������� ���������
//public:
//	Diagonal_Graph() {};//������ ���������� �� ���������
//	//bool GetGraphFromFile(const std::string file_name);//������� ������ ����� �� �����, ���������� true, ���� ������ ������ �������
//	//std::vector<Vertex> GetNeighbourVertex(const Vertex vertex)const;//�������, ������� ����� ���������� �������� �������� �������� ������� ��� ��������� �������
//};

#include <tuple>
#include <vector>
#include <string>

typedef std::pair<double, double> edge; //��������������� ��� "���� ����� � ��������� ������" � �������� ������. ����� ������ � ���� ���������� � �������

class Ants;

class Graph {
public:
	Graph() {}//����������� �� ���������
	Graph(std::vector<std::vector<edge>> matrix); //�����������
	bool GetGraphFromFile(const std::string filename); //������ ����� �� �����
	std::vector<std::vector<edge>> GetMatrix()const; //������-������� ��� ��������� ������� �����
	int GetSize();
	std::vector<std::vector<edge>>& GetOriginalMatrix();

	std::vector<std::vector<edge>> graph_matrix; //������� �����
};