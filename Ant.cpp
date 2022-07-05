#include"ant.h"
#include"Rondomizer.h"
#include<cmath>
#include<algorithm>
#include<iostream>

Ants::Ants(int count_of_ants, int importance_of_pheromone, int importance_of_distance, double evaporation_of_pheromone, double pheromones_from_ant) {
	ants_count = count_of_ants;  //заносим значение количества муравьёв
	pheromone_importance = importance_of_pheromone; //заносим значение важности феромона
	distance_importance = importance_of_distance; //заносим значение важности дистанции
	pheromones_evaporation = evaporation_of_pheromone; //заносим значение коэффициента испарения
	pheromone_from_ant = pheromones_evaporation; //заносим значение количества выделяемого одним муравьём феромона на одну дорогу
	vertex_count = 0;
	indexes_of_traveled_edges.resize(ants_count);
	best_length = 1000000;
}

bool Ants::InitGraph(const std::string filename, int beginning_vertex) {
	bool result = graph.GetGraphFromFile(filename); //вызываем функцию чтения графа из файла и возвращаем то что вернула функция GetGraphFromFile
	vertex_count = graph.GetSize();//заносим значение количества вершин
	for (int k = 0; k < ants_count; ++k) {
		indexes_of_traveled_edges[k].push_back(beginning_vertex);
	}
	return result;//возвращаем вывод функции getGraphFromFile
}

bool Ants::AntWasInVertex(int ant_num, int vertex_num) {
	return find(indexes_of_traveled_edges[ant_num].begin(), indexes_of_traveled_edges[ant_num].end(), vertex_num)
		!= indexes_of_traveled_edges[ant_num].end();//если вершина была пройдена муравьем, то возвращаем true, иначе - false
}

double Ants::ProbabilityOfVertex(int from_city, int to_city, int ant_num) {
	auto matrix = graph.GetMatrix();//получаем копию матрицы графа

	double Tij = std::pow(matrix[from_city][to_city].second, pheromone_importance);//вычисляем количество феромона на дуге, возведенное в степень
	double Nij = std::pow(1 / matrix[from_city][to_city].first, distance_importance);//вычисляем величину, обратную расстоянию, возведенную в степень

	double current_TijNij = Tij * Nij;//запоминаем "привлекательность" текущей вершины
	double sum = Tij * Nij;//переменная, хранящая "привлекательности" всех вершин

	if (matrix[from_city][to_city].first > 0) {
		for (int i = 0; i < vertex_count; ++i) {
			if (i != to_city && matrix[from_city][i].first > 0)
				if (!AntWasInVertex(ant_num, i)) {
					Tij = std::pow(matrix[from_city][to_city].second, pheromone_importance);
					Nij = std::pow(1 / matrix[from_city][to_city].first, distance_importance);
					sum += Tij * Nij; //прибавляем "привлекательность" i-й вершины
				}
		}
	}
	else return 0;

	return current_TijNij / sum;//вычисляем вероятность того, что текущая вершина будет "привлекательнее" других
}

int Ants::ChooseNextVertex(int ant_num) {
	int last_vertex_index = indexes_of_traveled_edges[ant_num][indexes_of_traveled_edges[ant_num].size() - 1];//получаем индекс последнего города, в котором муравей был

	auto prob_rand = GetProbabilityRand();//получаем случайное дробное число от 0 до 1
	int i = -1; //счетчик
	double prob_sum = 0; //переменная хранящая сумму вероятностей
	while (prob_sum < prob_rand) {//до тех пор пока наше случайное число больше суммы вероятностей счётчик будет увеличиваться
		++i;
		if (!AntWasInVertex(ant_num, i))
			prob_sum += ProbabilityOfVertex(last_vertex_index, i, ant_num);//прибавляем к сумме вероятность перехода в i-ую вершину
	}
	return i;//возвращаем счётчик, который отсчитал номер города, который выпал случайным выбором
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
				indexes_of_traveled_edges[k].push_back(ChooseNextVertex(k));//вводим в вектор индекс вершины, в которую муравей пришел на этом ходу
			}
		}//делаем это для всех муравьев с помощью цикла

		for (int i = 0; i < vertex_count; ++i) {
			for (int j = 0; j < vertex_count; ++j) {
				if (i != j) {
					graph.graph_matrix[i][j].second *= (1 - pheromones_evaporation);//проходим по всей матрице кроме диагональных элементов и вычитаем испаренные ферамоны
				}
			}
		}

		//далее мы будем прибавлять ферамоны к соответствующим рёбрам
		//в списке пройденных мершин хранится путь муравья, поэтому предыдущий и следующий элементы показывают откуда и куда пошел муравей
		for (int k = 0; k < ants_count; ++k) {
			if (indexes_of_traveled_edges[k].size() < vertex_count)
				continue;
			int index_of_first_visited_city = indexes_of_traveled_edges[k][0];
			int index_of_last_visited_city = indexes_of_traveled_edges[k][indexes_of_traveled_edges[k].size() - 1];

			double length = 0;

			for (int i = 0; i < indexes_of_traveled_edges[k].size() - 1; ++i) {

				int index_of_fromcity = indexes_of_traveled_edges[k][i];//предыдущий город, в котором был муравей
				int index_of_tocity = indexes_of_traveled_edges[k][i + 1];//город, в который муравей отправился из предыдущего города

				double distance_between_cities = graph.graph_matrix[index_of_fromcity][index_of_tocity].first;//расстояние между этими городами

				length += distance_between_cities;

				graph.graph_matrix[index_of_fromcity][index_of_tocity].second += pheromone_from_ant / distance_between_cities;//добавляем ферамон по формуле из теории
				graph.graph_matrix[index_of_tocity][index_of_fromcity].second += pheromone_from_ant / distance_between_cities;//добавляем в симметричную ячейку матрицы
			}

			length += graph.graph_matrix[index_of_first_visited_city][index_of_last_visited_city].first;

			if (length < best_length) {
				best_length = length;
				best_route = indexes_of_traveled_edges[k];
			}

			indexes_of_traveled_edges[k].resize(1); //обновляем пройденный путь для k-го муравья, оставляя только начальный город
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