//файл определения методов класса Graph
#include"Graph.h"
#include<fstream>

Graph::Graph(std::vector<std::vector<edge>> matrix) {
	graph_matrix = matrix;//обычный конструктор, который копирует в себя введенную матрицу 
}

bool Graph::GetGraphFromFile(const std::string filename) {
	std::ifstream file(filename); //открываем файл
	
	if (!file) return false;//проверяем открылся ли файл, возвращаем значение false, если файл не открылся 

	int vertex_count; //переменная, в которую будет введено количество вершин
	file >> vertex_count; //передаем значение количества вершин
	for (int i = 0; i < vertex_count; ++i) {
		std::vector<edge>new_row; //создаем вспомогательную переменную, хранящую в себе текущий вводимый ряд значений
		for (int j = 0; j < vertex_count; ++j) { 
			double new_distance; //значение длины ребра
			file >> new_distance;
			new_row.push_back(std::make_pair(new_distance, 0.1));//вставляем в ряд значения дистанций, при этом приравнивая значения феромонов к 0.1
		}
		graph_matrix.push_back(new_row); //вставляем вспомогательный ряд в основную матрицу
	}

	file.close();//закрываем файл
	return true;//возвращаем значение true, чтоб показать, что чтение прошло успешно
}

std::vector<std::vector<edge>> Graph::GetMatrix() const{
	return graph_matrix;//обычная функция-геттер, которая имеет модификатор const, тк мы не меняем никакие значения
}

int Graph::GetSize() {
	return graph_matrix.size();
}

std::vector<std::vector<edge>>& Graph::GetOriginalMatrix() {
	return graph_matrix;
}