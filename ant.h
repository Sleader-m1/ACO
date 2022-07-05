#pragma once

#include"Graph.h"

class Ants { //класс муравей
	Graph graph;

	int pheromone_importance,
		distance_importance; //значения переменных алфа и бетта из формулы вероятности соответственно
	double pheromones_evaporation,//коэффициент испарения феормона
		pheromone_from_ant; // феромон, оставляемый муравьем на всю дорогу

	int ants_count, //количество муравьев в итерации
		vertex_count;//количество вершин графа

	double best_length; //кротчайшая длина пройденного пути

	std::vector<std::vector<int>> indexes_of_traveled_edges; //список списков пройденных вершин
	std::vector<int> best_route;
public:
	Ants(int count_of_ants, int importance_of_pheromone, int importance_of_distance, double evaporation_of_pheromone, double pheromones_from_ant);
	int ChooseNextVertex(int ant_num); // функция, выбирающая для муравья следующую вершину
	void MakeIteratoin(int iteration_count);//функция, выполняющая указанное аргументом количество итераций
	double ProbabilityOfVertex(int from_city, int to_city, int ant_num);//функция, возвращающая вероятность того, что муравей пойдет по пути from_city-to_city
	bool InitGraph(const std::string filename, int beginning_vertex);//функция сеттер для графа, является оберткой для функции чтения графа из файла
	bool AntWasInVertex(int ant_num, int vertex_num);//функция, возвращающая true, если муравей проходил по заданному маршруту, false-в противном случае
	int	HaveValidVertex(int ant_num);//фукнция, возвращающая true, если муравей не зашел в "тупик", в противном случае-false
	void PrintMatrix();
};

