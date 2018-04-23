#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct comp {
    int visit;
    int value;
} s_comp;

int validation_check(int N, int M) {
    if (!((N >= 0) && (N <= 5000))) {
        printf("bad number of vertices");
        return 0;
    }
    if (!((M >= 0) && (M <= N * (N - 1) / 2))) {
        printf("bad number of edges");
        return 0;
    }
    return 1;
}

void dijkstra_alg(int M, int N, int F, int *t, int **graph_matrix, s_comp *arr, int *g, int *temp) {
    while (*t < M) {
        for (int i = 0; i < N; i++) {
            if (graph_matrix[*g][i] > 0) {
                if (graph_matrix[*g][i] + arr[*g].value == INT_MAX) {
                    arr[i].value = graph_matrix[*g][i] + arr[*g].value;
                    (*t)++;
                } else if ((abs(graph_matrix[*g][i] + arr[*g].value) < arr[i].value) && (arr[i].visit == 0)) {
                    arr[i].value = graph_matrix[*g][i] + arr[*g].value;
                    (*t)++;
                }
            }
        }
        arr[*g].visit = 1;
        for (int i = 0; i < N; i++) {
            if ((graph_matrix[*g][i] > 0) && (arr[i].visit == 0)) {
                if (i != F - 1) {
                    *temp = *g;
                    *g = i;
                    dijkstra_alg(M, N, F, t, graph_matrix, arr, g, temp);
                    *g = *temp;
                } else {
                    for (int c = 0; c < N; c++) {
                        if ((arr[c].visit == 0) && (c != F - 1)) {
                            *g = c;
                            dijkstra_alg(M, N, F, t, graph_matrix, arr, g, temp);
                        }
                    }
                }
            }
        }
    }
}

int values_check(int start, int end, int length, int N) {
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

int create_graph_matrix(int vertices_amount, int edges_amount, int **graph_matrix, FILE *infile, int S) {
    for (int i = 0; i < vertices_amount; i++) {
        graph_matrix[i] = (int *) malloc(vertices_amount * sizeof(int));
    }
    for (int i = 0; i < vertices_amount; i++) {
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

int main(int argc, char *argv[]) {
    int N, M, F, S;

    FILE *infile = fopen(argv[1], "rb");

    fscanf(infile, "%d %d %d %d", &N, &S, &F, &M);
    if (validation_check(N, M) == 0)
        return 0;

    int **graph_matrix = (int **) malloc(N * sizeof(int *));

    if (create_graph_matrix(N, M, graph_matrix, infile, S) == -1)
        return 0;

    s_comp *arr = (s_comp *) malloc(sizeof(s_comp) * N);

    for (int i = 0; i < N; i++) {
        arr[i].visit = 0;
        arr[i].value = INT_MAX;
    }
    arr[S - 1].value = 0;

    int g = S - 1;
    int g1 = g;
    int t = 0;
    int temp = 0;

    int *arr2 = (int *) calloc(sizeof(int), M);
    arr2[0] = g;
    int u = 1;

    if (N % 2 == 0) {
        dijkstra_alg(M - 1, N, F, &t, graph_matrix, arr, &g, &temp);
    } else dijkstra_alg(M, N, F, &t, graph_matrix, arr, &g, &temp);

    for (int i = 0; i < N; i++) {
        if (arr[i].value >= 0)
            printf("%d\n", arr[i].value);
        else printf("INT_MAX+ ");
    }

    if (arr[F - 1].value == 0) {
        printf("no path");
    }
    return 0;
}