#pragma once

//#include<tuple>
//#include<vector>
//#include<iostream>
//
//
//class Vertex {//класс вершина для удобного хранения
//	double pos_x, pos_y; //координата вершины
//	bool isEmpty; //булеан, показывающий тип вершины: пустой или реальный
//	short int vertex_id; //опознавательный номер вершины
//public:
//	Vertex(double x, double y, short int id): pos_x(x), pos_y(y), isEmpty(true), vertex_id(id){}
//	Vertex(short int id):isEmpty(false), pos_x(-1), pos_y(-1), vertex_id(id) {}
//	std::pair<double, double> GetCoordinates() const; //геттер-функция, которая оставляет все значения неизменными
//	bool GetTypeOfVertex()const; //геттер-функция, дающая понимание о том является ли вершина пустой
//};
//
//class Edge {//класс ребро
//	Vertex fvertex, svertex; //вершины, которые соединяет ребро
//	double smell; //показатель "силы запаха феромонов" ребра
//	double distance; //вес ребра, который эффективнее хранить чем каждый раз вычислять 
//public:
//	Edge(Vertex first_vertex, Vertex second_vertex):fvertex(first_vertex), svertex(second_vertex), smell(0){}
//	std::pair<Vertex, Vertex> Getvertexes() const; //геттер-функция для получения вершин без изменения значений
//};
//
//class Graph {//класс граф
//protected:
//	std::vector<Edge> edges;
//	std::vector<Vertex> vertexes;
//public:
//	Graph() {};//пустой констуктор по умолчанию
//	bool GetGraphFromFile(const std::string file_name)const;//функция чтения графа из файла, возвращает true, если чтение прошло успешно
//	std::vector<Vertex> GetNeighbourVertex(const Vertex vertex)const;//функция, которая будет возвращать муравьям соседние непустые вершины для введенной вершины
//};
//
////создадим отдельно граф с диагональными ребрами, чтоб не нагружать систему 
////постоянными проверками "можно ли идти по диагонали?"
//
//class Diagonal_Graph:public Graph{//класс графа, разрешающего передвижение по диагонали
//	std::vector<std::vector<Vertex>> graph_matrix; //матрица графа, элементы которой являются вершинами
//public:
//	Diagonal_Graph() {};//пустой констуктор по умолчанию
//	//bool GetGraphFromFile(const std::string file_name);//функция чтения графа из файла, возвращает true, если чтение прошло успешно
//	//std::vector<Vertex> GetNeighbourVertex(const Vertex vertex)const;//функция, которая будет возвращать муравьям соседние непустые вершины для введенной вершины
//};

#include <tuple>
#include <vector>
#include <string>

typedef std::pair<double, double> edge; //переименовываем тип "пара чисел с плавающей точкой" и называем ребром. Ребро хранит в себе расстояние и феромон

class Ants;

class Graph {
public:
	Graph() {}//конструктор по умолчанию
	Graph(std::vector<std::vector<edge>> matrix); //конструктор
	bool GetGraphFromFile(const std::string filename); //чтение графа из файла
	std::vector<std::vector<edge>> GetMatrix()const; //геттер-функция для получения матрицы графа
	int GetSize();
	std::vector<std::vector<edge>>& GetOriginalMatrix();

	std::vector<std::vector<edge>> graph_matrix; //матрица графа
};