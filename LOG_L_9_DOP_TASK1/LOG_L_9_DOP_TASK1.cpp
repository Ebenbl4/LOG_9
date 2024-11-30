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

typedef struct Node {
	int vertex;
	struct Node* next;
} Node;

typedef struct Queue {
	int* items;
	int front;
	int rear;
	int capacity;
} Queue;

int_fast8_t** create_graph(int size);
void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
Node** conv_to_adj_list(int size, int_fast8_t** graph);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
void BFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST);
void find_distances_breath_matrix(int size, int_fast8_t** graph_pointer, int start_vertex);
void DFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST, int current_dist);
void find_distances_depth_matrix(int size, int_fast8_t** graph_pointer, int start_vertex);
void find_distances_breath_adjList(Node** adj_list, int size, int start_vertex);
void find_distances_depth_adjList(Node** adj_list, int size, int start_vertex);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** adj_matrix;
	int size, start_v;
	float edge_probability;
	clock_t search1_start, search1_end, search2_start, search2_end, search3_start, search3_end, search4_start, search4_end;
	double search_time1, search_time2, search_time3, search_time4;

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

	adj_matrix = create_graph(size);
	srand((unsigned int)time(NULL));
	initialize_graph(size, adj_matrix, edge_probability);
	//printf("Матрица смежности: \n\n");
	//print_graph(size, adj_matrix);

	//printf("\nСписок смежности: \n\n");
	Node** adj_list = conv_to_adj_list(size, adj_matrix);
	//print_adj_list(size, adj_list);

	printf("\nВведите начальную вершину для поиска расстояний: ");
	scanf("%d", &start_v);

	search1_start = clock();
	find_distances_breath_matrix(size, adj_matrix, start_v);
	search1_end = clock();
	search_time1 = ((double)(search1_end - search1_start)) / CLOCKS_PER_SEC * 1000;
	

	search2_start = clock();
	find_distances_depth_matrix(size, adj_matrix, start_v);
	search2_end = clock();
	search_time2 = ((double)(search2_end - search2_start)) / CLOCKS_PER_SEC * 1000;
	

	search3_start = clock();
	find_distances_breath_adjList(adj_list, size, start_v);
	search3_end = clock();
	search_time3 = ((double)(search3_end - search3_start)) / CLOCKS_PER_SEC * 1000;
	

	search4_start = clock();
	find_distances_depth_adjList(adj_list, size, start_v);
	search4_end = clock();
	search_time4 = ((double)(search4_end - search4_start)) / CLOCKS_PER_SEC * 1000;
	printf("\nВремя обхода матрицы в ширину: %f ms\n", search_time1);
	printf("\nВремя обхода матрицы в глубину: %f ms\n", search_time2);
	printf("\nВремя обхода списка в ширину: %f ms\n", search_time3);
	printf("\nВремя обхода списка в глубину: %f ms\n", search_time4);

	for (int i = 0; i < size; i++) {
		free(adj_matrix[i]);
	}
	free(adj_matrix);
	free_adj_list(size, adj_list);
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

Node** conv_to_adj_list(int size, int_fast8_t** graph) {
	Node** adj_list = (Node**)malloc(size * sizeof(Node*));

	for (int i = 0; i < size; i++) {
		adj_list[i] = NULL;
	}

	for (int i = 0; i < size; i++) {
		Node* last = NULL;

		for (int j = 0; j < size; j++) {
			if (graph[i][j]) {
				Node* new_node = (Node*)malloc(sizeof(Node));
				new_node->vertex = j;
				new_node->next = NULL;

				if (adj_list[i] == NULL) {
					adj_list[i] = new_node;
				}

				else {
					last->next = new_node;
				}
				last = new_node;
			}
		}
	}
	return adj_list;
}

void print_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		//printf("%d: ", i + 1);
		Node* current = adj_list[i];
		while (current != NULL) {
			//printf("%d ", current->vertex + 1);
			current = current->next;
		}
		//printf("\n");
	}
}

void free_adj_list(int size, Node** adj_list) {
	for (int i = 0; i < size; i++) {
		Node* current = adj_list[i];
		while (current != NULL) {
			Node* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(adj_list);
}

void find_distances_breath_matrix(int size, int_fast8_t** graph_pointer, int start_vertex) {
	start_vertex--;
	int* DIST = new int[size];
	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	//printf("\nПорядок обхода матрицы в ширину: ");
	BFSD(start_vertex, size, graph_pointer, DIST);

	//printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	//for (int i = 0; i < size; i++) {
	//	printf("До вершины %d: %d\n", i + 1, DIST[i]);
	//}

	delete[] DIST;
}

void BFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST) {
	queue<int> Q;

	Q.push(v);
	DIST[v] = 0;

	while (!Q.empty()) {
		int current = Q.front();
		Q.pop();
		//printf("%d ", current + 1);

		for (int i = 0; i < size; i++) {
			if (graph_pointer[current][i] == 1 && DIST[i] == -1) {
				Q.push(i);
				DIST[i] = DIST[current] + 1;
			}
		}
	}
}

void find_distances_depth_matrix(int size, int_fast8_t** graph_pointer, int start_vertex) {
	start_vertex--;
	int* DIST = new int[size];
	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	//printf("\nПорядок обхода матрицы в глубину: ");
	DFSD(start_vertex, size, graph_pointer, DIST, 0);

	//printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	//for (int i = 0; i < size; i++) {
		//printf("До вершины %d: %d\n", i + 1, DIST[i]);
	//}

	delete[] DIST;
}

void DFSD(int v, int size, int_fast8_t** graph_pointer, int* DIST, int current_dist) {
	//printf("%d ", v + 1);
	DIST[v] = current_dist;

	for (int i = 0; i < size; i++) {
		if (graph_pointer[v][i] == 1 && DIST[i] == -1) {
			DFSD(i, size, graph_pointer, DIST, current_dist + 1);
		}
	}
}

Queue* createQueue(int capacity) {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->items = (int*)malloc(capacity * sizeof(int));
	queue->front = -1;
	queue->rear = -1;
	queue->capacity = capacity;
	return queue;
}

int isEmpty(Queue* queue) {
	return queue->front == -1;
}

void enqueue(Queue* queue, int value) {
	if (queue->front == -1)
		queue->front = 0;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->items[queue->rear] = value;
}

int dequeue(Queue* queue) {
	int item = queue->items[queue->front];
	if (queue->front == queue->rear) {
		queue->front = -1;
		queue->rear = -1;
	}
	else {
		queue->front = (queue->front + 1) % queue->capacity;
	}
	return item;
}

void find_distances_breath_adjList(Node** adj_list, int size, int start_vertex) {
	int* DIST = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	Queue* Q = createQueue(size);
	start_vertex--;
	enqueue(Q, start_vertex);
	DIST[start_vertex] = 0;

	//printf("\nПорядок обхода списка в ширину: ");
	while (!isEmpty(Q)) {
		int v = dequeue(Q);
		//printf("%d ", v + 1);

		Node* current = adj_list[v];
		while (current != NULL) {
			int i = current->vertex;
			if (DIST[i] == -1) {
				enqueue(Q, i);
				DIST[i] = DIST[v] + 1;
			}
			current = current->next;
		}
	}

	//printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	//for (int i = 0; i < size; i++) {
	//	printf("До вершины %d: %d\n", i + 1, DIST[i]);
	//}

	free(DIST);
	free(Q->items);
	free(Q);
}

void DFS(Node** adj_list, int vertex, int* DIST, int current_dist) {
	DIST[vertex] = current_dist;
	//printf("%d ", vertex + 1);

	Node* current = adj_list[vertex];
	while (current != NULL) {
		int next_vertex = current->vertex;
		if (DIST[next_vertex] == -1) {
			DFS(adj_list, next_vertex, DIST, current_dist + 1);
		}
		current = current->next;
	}
}

void find_distances_depth_adjList(Node** adj_list, int size, int start_vertex) {
	int* DIST = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++) {
		DIST[i] = -1;
	}

	start_vertex--;
	//printf("\nПорядок обхода списка в глубину: ");
	DFS(adj_list, start_vertex, DIST, 0);

	//printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
	for (int i = 0; i < size; i++) {
		//printf("До вершины %d: %d\n", i + 1, DIST[i]);
	}

	free(DIST);
}