#include"ant.h"
#include"Rondomizer.h"
#include<cmath>
#include<algorithm>
#include<iostream>

Ants::Ants(int count_of_ants, int importance_of_pheromone, int importance_of_distance, double evaporation_of_pheromone, double pheromones_from_ant) {
	ants_count = count_of_ants;  //çàíîñèì çíà÷åíèå êîëè÷åñòâà ìóðàâü¸â
	pheromone_importance = importance_of_pheromone; //çàíîñèì çíà÷åíèå âàæíîñòè ôåðîìîíà
	distance_importance = importance_of_distance; //çàíîñèì çíà÷åíèå âàæíîñòè äèñòàíöèè
	pheromones_evaporation = evaporation_of_pheromone; //çàíîñèì çíà÷åíèå êîýôôèöèåíòà èñïàðåíèÿ
	pheromone_from_ant = pheromones_evaporation; //çàíîñèì çíà÷åíèå êîëè÷åñòâà âûäåëÿåìîãî îäíèì ìóðàâü¸ì ôåðîìîíà íà îäíó äîðîãó
	vertex_count = 0;
	indexes_of_traveled_edges.resize(ants_count);
	best_length = 1000000;
}

bool Ants::InitGraph(const std::string filename, int beginning_vertex) {
	bool result = graph.GetGraphFromFile(filename); //âûçûâàåì ôóíêöèþ ÷òåíèÿ ãðàôà èç ôàéëà è âîçâðàùàåì òî ÷òî âåðíóëà ôóíêöèÿ GetGraphFromFile
	vertex_count = graph.GetSize();//çàíîñèì çíà÷åíèå êîëè÷åñòâà âåðøèí
	for (int k = 0; k < ants_count; ++k) {
		indexes_of_traveled_edges[k].push_back(beginning_vertex);
	}
	return result;//âîçâðàùàåì âûâîä ôóíêöèè getGraphFromFile
}

bool Ants::AntWasInVertex(int ant_num, int vertex_num) {
	return find(indexes_of_traveled_edges[ant_num].begin(), indexes_of_traveled_edges[ant_num].end(), vertex_num)
		!= indexes_of_traveled_edges[ant_num].end();//åñëè âåðøèíà áûëà ïðîéäåíà ìóðàâüåì, òî âîçâðàùàåì true, èíà÷å - false
}

double Ants::ProbabilityOfVertex(int from_city, int to_city, int ant_num) {
	auto matrix = graph.GetMatrix();//ïîëó÷àåì êîïèþ ìàòðèöû ãðàôà

	double Tij = std::pow(matrix[from_city][to_city].second, pheromone_importance);//âû÷èñëÿåì êîëè÷åñòâî ôåðîìîíà íà äóãå, âîçâåäåííîå â ñòåïåíü
	double Nij = std::pow(1 / matrix[from_city][to_city].first, distance_importance);//âû÷èñëÿåì âåëè÷èíó, îáðàòíóþ ðàññòîÿíèþ, âîçâåäåííóþ â ñòåïåíü

	double current_TijNij = Tij * Nij;//çàïîìèíàåì "ïðèâëåêàòåëüíîñòü" òåêóùåé âåðøèíû
	double sum = Tij * Nij;//ïåðåìåííàÿ, õðàíÿùàÿ "ïðèâëåêàòåëüíîñòè" âñåõ âåðøèí

	if (matrix[from_city][to_city].first > 0) {
		for (int i = 0; i < vertex_count; ++i) {
			if (i != to_city && matrix[from_city][i].first > 0)
				if (!AntWasInVertex(ant_num, i)) {
					Tij = std::pow(matrix[from_city][i].second, pheromone_importance);
					Nij = std::pow(1 / matrix[from_city][i].first, distance_importance);
					sum += Tij * Nij; //ïðèáàâëÿåì "ïðèâëåêàòåëüíîñòü" i-é âåðøèíû
				}
		}
	}
	else return 0;

	return current_TijNij / sum;//âû÷èñëÿåì âåðîÿòíîñòü òîãî, ÷òî òåêóùàÿ âåðøèíà áóäåò "ïðèâëåêàòåëüíåå" äðóãèõ
}

int Ants::ChooseNextVertex(int ant_num) {
	int last_vertex_index = indexes_of_traveled_edges[ant_num][indexes_of_traveled_edges[ant_num].size() - 1];//ïîëó÷àåì èíäåêñ ïîñëåäíåãî ãîðîäà, â êîòîðîì ìóðàâåé áûë

	auto prob_rand = GetProbabilityRand();//ïîëó÷àåì ñëó÷àéíîå äðîáíîå ÷èñëî îò 0 äî 1
	int i = -1; //ñ÷åò÷èê
	double prob_sum = 0; //ïåðåìåííàÿ õðàíÿùàÿ ñóììó âåðîÿòíîñòåé
	while (prob_sum < prob_rand) {//äî òåõ ïîð ïîêà íàøå ñëó÷àéíîå ÷èñëî áîëüøå ñóììû âåðîÿòíîñòåé ñ÷¸ò÷èê áóäåò óâåëè÷èâàòüñÿ
		++i;
		if (!AntWasInVertex(ant_num, i))
			prob_sum += ProbabilityOfVertex(last_vertex_index, i, ant_num);//ïðèáàâëÿåì ê ñóììå âåðîÿòíîñòü ïåðåõîäà â i-óþ âåðøèíó
	}
	return i;//âîçâðàùàåì ñ÷¸ò÷èê, êîòîðûé îòñ÷èòàë íîìåð ãîðîäà, êîòîðûé âûïàë ñëó÷àéíûì âûáîðîì
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
				indexes_of_traveled_edges[k].push_back(ChooseNextVertex(k));//ââîäèì â âåêòîð èíäåêñ âåðøèíû, â êîòîðóþ ìóðàâåé ïðèøåë íà ýòîì õîäó
			}
		}//äåëàåì ýòî äëÿ âñåõ ìóðàâüåâ ñ ïîìîùüþ öèêëà

		for (int i = 0; i < vertex_count; ++i) {
			for (int j = 0; j < vertex_count; ++j) {
				if (i != j) {
					graph.graph_matrix[i][j].second *= (1 - pheromones_evaporation);//ïðîõîäèì ïî âñåé ìàòðèöå êðîìå äèàãîíàëüíûõ ýëåìåíòîâ è âû÷èòàåì èñïàðåííûå ôåðàìîíû
				}
			}
		}

		//äàëåå ìû áóäåì ïðèáàâëÿòü ôåðàìîíû ê ñîîòâåòñòâóþùèì ð¸áðàì
		//â ñïèñêå ïðîéäåííûõ ìåðøèí õðàíèòñÿ ïóòü ìóðàâüÿ, ïîýòîìó ïðåäûäóùèé è ñëåäóþùèé ýëåìåíòû ïîêàçûâàþò îòêóäà è êóäà ïîøåë ìóðàâåé
		for (int k = 0; k < ants_count; ++k) {
			if (indexes_of_traveled_edges[k].size() < vertex_count){
				indexes_of_traveled_edges[k].resize(1);
				continue;
			}
			int index_of_first_visited_city = indexes_of_traveled_edges[k][0];
			int index_of_last_visited_city = indexes_of_traveled_edges[k][indexes_of_traveled_edges[k].size() - 1];

			double length = 0;

			for (int i = 0; i < indexes_of_traveled_edges[k].size() - 1; ++i) {

				int index_of_fromcity = indexes_of_traveled_edges[k][i];//ïðåäûäóùèé ãîðîä, â êîòîðîì áûë ìóðàâåé
				int index_of_tocity = indexes_of_traveled_edges[k][i + 1];//ãîðîä, â êîòîðûé ìóðàâåé îòïðàâèëñÿ èç ïðåäûäóùåãî ãîðîäà

				double distance_between_cities = graph.graph_matrix[index_of_fromcity][index_of_tocity].first;//ðàññòîÿíèå ìåæäó ýòèìè ãîðîäàìè

				length += distance_between_cities;

				graph.graph_matrix[index_of_fromcity][index_of_tocity].second += pheromone_from_ant / distance_between_cities;//äîáàâëÿåì ôåðàìîí ïî ôîðìóëå èç òåîðèè
				graph.graph_matrix[index_of_tocity][index_of_fromcity].second += pheromone_from_ant / distance_between_cities;//äîáàâëÿåì â ñèììåòðè÷íóþ ÿ÷åéêó ìàòðèöû
			}

			length += graph.graph_matrix[index_of_first_visited_city][index_of_last_visited_city].first;

			if (length < best_length) {
				best_length = length;
				best_route = indexes_of_traveled_edges[k];
			}

			indexes_of_traveled_edges[k].resize(1); //îáíîâëÿåì ïðîéäåííûé ïóòü äëÿ k-ãî ìóðàâüÿ, îñòàâëÿÿ òîëüêî íà÷àëüíûé ãîðîä
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
