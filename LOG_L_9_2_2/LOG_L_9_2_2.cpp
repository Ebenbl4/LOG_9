#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <locale>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

Node** create_adj_list(int size);
void initialize_adj_list(int size, Node** adj_list, float edge_prob);
void print_adj_list(int size, Node** adj_list);
void free_adj_list(int size, Node** adj_list);
void add_edge(Node** adj_list, int src, int dest);
void find_distances(Node** adj_list, int size, int start_vertex);

int main(void) {
    setlocale(LC_ALL, "Russian");
    Node** adj_list;
    int size;
    float edge_prob;

    printf("Введите количество вершин графа: ");
    if (!scanf("%d", &size) || size <= 1) {
        fprintf(stderr, "Ошибка ввода размера\n");
        exit(1);
    }

    adj_list = create_adj_list(size);
    printf("Введите вероятность создания ребра (от 0 до 100): ");
    if (!scanf("%f", &edge_prob) || edge_prob < 0 || edge_prob > 100) {
        fprintf(stderr, "Ошибка ввода вероятности\n");
        exit(1);
    }

    initialize_adj_list(size, adj_list, edge_prob);
    printf("\nСписок смежности графа:\n\n");
    print_adj_list(size, adj_list);

    int start_vertex;
    printf("\nВведите начальную вершину (от 1 до %d): ", size);
    scanf("%d", &start_vertex);
    start_vertex--;

    if (start_vertex >= 0 && start_vertex < size) {
        find_distances(adj_list, size, start_vertex);
    }
    else {
        printf("Некорректный номер вершины\n");
    }

    free_adj_list(size, adj_list);
    return 0;
}

void print_adj_list(int size, Node** adj_list) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i + 1);
        Node* current = adj_list[i];
        while (current != NULL) {
            printf("%d ", current->vertex + 1);
            current = current->next;
        }
        printf("\n");
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

Node** create_adj_list(int size) {
    Node** adj_list = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        adj_list[i] = NULL;
    }
    return adj_list;
}

void add_edge(Node** adj_list, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = NULL;

    if (adj_list[src] == NULL) {
        adj_list[src] = newNode;
    }
    else {
        Node* current = adj_list[src];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void initialize_adj_list(int size, Node** adj_list, float edge_prob) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_prob) {
                add_edge(adj_list, i, j);
                add_edge(adj_list, j, i);
            }
        }
    }
}

void DFS(Node** adj_list, int vertex, int* DIST, int current_dist) {
    DIST[vertex] = current_dist;
    printf("%d ", vertex + 1);

    Node* current = adj_list[vertex];
    while (current != NULL) {
        int next_vertex = current->vertex;
        if (DIST[next_vertex] == -1) {
            DFS(adj_list, next_vertex, DIST, current_dist + 1);
        }
        current = current->next;
    }
}

void find_distances(Node** adj_list, int size, int start_vertex) {
    int* DIST = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        DIST[i] = -1;
    }

    printf("\nПорядок обхода в глубину: ");
    DFS(adj_list, start_vertex, DIST, 0);

    printf("\n\nРасстояния от вершины %d до остальных вершин:\n\n", start_vertex + 1);
    for (int i = 0; i < size; i++) {
        printf("До вершины %d: %d\n", i + 1, DIST[i]);
    }

    free(DIST);
}