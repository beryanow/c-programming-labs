#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int validation_check (int N, int M) {
    if (!((N >= 0) && (N <= 5000))) {
        printf("bad number of vertices");
        return 0;
    }
    if (!((M >= 0) && (M <= N * (N - 1) / 2))) {
        printf("bad number of edges");
        return 0;
    }
    if (N - 1 > M) {
        printf("no spanning tree");
        return 0;
    }
    return 1;
}

int values_check (int start, int end, int length, int N) {
    if (!((start >= 1) && (start <= N) && (end >= 1) && (end <= N))) {
        printf("bad vertex");
        return 0;
    }
    if (!((length >= 0) && (length <= INT_MAX))) {
        printf("bad length");
        return 0;
    }
    return 1;
}

void find_min_spanning_tree_using_prim_algorithm(int vertices_amount, int* str_num, int* col_num, int* str_num1, int* col_num1, int* min, int** G, int* used_vertices) {
    int finds_amount = 0;
    while (finds_amount != vertices_amount - 1) {
        printf("%d %d\n", *str_num + 1, *col_num + 1);
        for (int i = 0; i < vertices_amount; i++) {
            if (i != *col_num)
                G[*str_num][i] = 0;
            G[*col_num][i] = 0;
        }
        *min = INT_MAX;
        for (int i = 0; i < vertices_amount; i++) {
            for (int j = 0; j < vertices_amount; j++) {
                if ((G[i][j] > 0) && (G[i][j] <= *min) &&
                        ((used_vertices[i] == 1) &&(used_vertices[j] == 0)) |
                                ((used_vertices[i] == 0) &&(used_vertices[j] == 1)) && (((i != *str_num) | (j != *col_num)))) {
                    *min = G[i][j];
                    *str_num1 = i;
                    *col_num1 = j;
                }
            }
        }
        *col_num = *col_num1;
        *str_num = *str_num1;
        used_vertices[*col_num] = 1;
        used_vertices[*str_num] = 1;
        finds_amount++;
    }
}

void find_min_graph_arc(int vertices_amount, int** graph_matrix, int* min, int* str_num, int* col_num, int* used_vertices) {
    for (int i = 0; i < vertices_amount; i++) {
        for (int j = 0; j < vertices_amount; j++) {
            if ((graph_matrix[i][j] > 0) && (graph_matrix[i][j] < *min)) {
                *min = graph_matrix[i][j];
                *str_num = i;
                *col_num = j;
            }
        }
    }
    used_vertices[*col_num] = 1;
    used_vertices[*str_num] = 1;
}

int create_graph_matrix(int vertices_amount, int edges_amount, int **graph_matrix, int *used_vertices, FILE* infile) {
    for (int i = 0; i < vertices_amount; i++) {
        graph_matrix[i] = (int *) malloc(vertices_amount * sizeof(int));
    }
    for (int i = 0; i < vertices_amount; i++) {
        used_vertices[i] = 0;
        for (int j = 0; j < vertices_amount; j++) {
            graph_matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < edges_amount; i++) {
        int start = -1, end = -1, length = -1;
        fscanf(infile, "%d %d %d", &start, &end, &length);
        if (start == -1) {
            printf("bad number of lines");
            return -1;
        }
        if (values_check(start, end, length, vertices_amount) == 0) {
            return -1;
        }
        graph_matrix[start - 1][end - 1] = length;
        graph_matrix[end - 1][start - 1] = length;

    }
    return 0;
}

int main(int argc, char* argv[]) {
    int N, M;
    FILE* infile = fopen(argv[1], "rb");

    fscanf(infile, "%d %d", &N, &M);
    if (validation_check(N, M) == 0)
        return 0;

    int** graph_matrix = (int**) malloc(N * sizeof(int*));
    int* used_vertices = (int*)malloc(N * sizeof(int));

    if (create_graph_matrix(N, M, graph_matrix, used_vertices, infile) == -1)
        return 0;

    int min = INT_MAX, str_num, col_num, str_num1, col_num1;

    find_min_graph_arc(N, graph_matrix, &min, &str_num, &col_num, used_vertices);
    find_min_spanning_tree_using_prim_algorithm(N, &str_num, &col_num, &str_num1, &col_num1, &min, graph_matrix, used_vertices);

    return 0;
}
