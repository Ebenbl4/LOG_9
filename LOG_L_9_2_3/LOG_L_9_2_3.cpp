#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>
#include <queue>

using namespace std;
int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void BFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST);
void find_distances_breath(int size, int_fast8_t** graph_pointer, int start_vertex);
void DFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST, int current_dist);
void find_distances_depth(int size, int_fast8_t** graph_pointer, int start_vertex);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph;
	int size, start_v;
	float edge_probability;
	clock_t search1_start, search1_end, search2_start, search2_end;
	double search_time1, search_time2;

	fprintf(stdout, "Введите количество вершин графа: ");
	if (!scanf("%d", &size) || size <= 1) {
		fprintf(stderr, "Ошибка ввода размера массива");
		exit(1);
	}

	fprintf(stdout, "Введите вероятность создания ребра (от 0 до 100): ");
	if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	graph = create_graph(size);
	srand((unsigned int)time(NULL));
	initialize_graph(size, graph, edge_probability);
	printf("Матрица смежности: \n\n");
	print_graph(size, graph);

	printf("\nВведите начальную вершину для поиска расстояний: ");
	scanf("%d", &start_v);

	search1_start = clock();
	find_distances_breath(size, graph, start_v);
	search1_end = clock();
	search_time1 = ((double)(search1_end - search1_start)) / CLOCKS_PER_SEC * 1000;
	printf("\nВремя обхода: %f ms\n", search_time1);

	search2_start = clock();
	find_distances_depth(size, graph, start_v);
	search2_end = clock();
	search_time2 = ((double)(search2_end - search2_start)) / CLOCKS_PER_SEC * 1000;
	printf("\nВремя обхода: %f ms\n", search_time2);

	for (int i = 0; i < size; i++) {
		free(graph[i]);
	}
	free(graph);

	return 0;
}

int_fast8_t** create_graph(int size) {
	int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
	for (int i = 0; i < size; i++) {
		array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
	}
	if (array == NULL) {
		fprintf(stderr, "Ошибка создания массива");
		exit(1);
	}
	return array;
}

void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = 1;
				graph_pointer[j][i] = 1;
			}
			else {
				graph_pointer[i][j] = 0;
				graph_pointer[j][i] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void print_graph(int size, int_fast8_t** graph_pointer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(stdout, "%d ", graph_pointer[j][i]);
		}
		fprintf(stdout, "\n");
	}
}

void find_distances_breath(int size, int_fast8_t** graph_pointer, int start_vertex) {
	start_vertex--;
	int* DIST = new int[size];
	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	printf("\nПорядок обхода в ширину: ");
	BFSD(start_vertex, size, graph_pointer, DIST);

	printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	for (int i = 0; i < size; i++) {
		printf("До вершины %d: %d\n", i + 1, DIST[i]);
	}

	delete[] DIST;
}

void BFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST) {
	queue<int> Q;

	Q.push(v);
	DIST[v] = 0;

	while (!Q.empty()) {
		int current = Q.front();
		Q.pop();
		printf("%d ", current + 1);

		for (int i = 0; i < size; i++) {
			if (graph_pointer[current][i] == 1 && DIST[i] == -1) {
				Q.push(i);
				DIST[i] = DIST[current] + 1;
			}
		}
	}
}

void find_distances_depth(int size, int_fast8_t** graph_pointer, int start_vertex) {
	start_vertex--;
	int* DIST = new int[size];
	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	printf("\nПорядок обхода в глубину: ");
	DFSD(start_vertex, size, graph_pointer, DIST, 0);

	printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	for (int i = 0; i < size; i++) {
		printf("До вершины %d: %d\n", i + 1, DIST[i]);
	}

	delete[] DIST;
}

void DFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST, int current_dist) {
	printf("%d ", v + 1);
	DIST[v] = current_dist;

	for (int i = 0; i < size; i++) {
		if (graph_pointer[v][i] == 1 && DIST[i] == -1) {
			DFSD(i, size, graph_pointer, DIST, current_dist + 1);
		}
	}
}