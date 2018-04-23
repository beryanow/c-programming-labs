#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct compose {
    int length;
    int start;
    int end;
    int val;
} s_compose;

void marking_as_read(int t, s_compose *arr, int *fin1, int y, int y2, int c) {
    for (int v = 0; v <= t; v++) {
        if (((arr[v].start == fin1[y]) && (arr[v].end == fin1[y2])) || (
                (arr[v].start == fin1[y2]) && (arr[v].end == fin1[y])))
            arr[v].val = c;
    }
}

s_compose *sorting_edges(FILE *infile, int *t, int *mistake) {

    int N, M;
    fscanf(infile, "%d %d", &N, &M);
    int start, end, len;

    if (!((N >= 0) && (N <= 5000))) {
        *mistake = 1;
        printf("bad number of vertices\n");
    }

    if (!((M >= 0) && (M <= N * (N + 1) / 2))) {
        *mistake = 1;
        printf("bad number of edges\n");
    }

    s_compose *arr = (s_compose *) malloc(sizeof(s_compose) * M);

    for (int i = 2; i < M + 2; i++) {
        start = -1;
        end = -1;
        len = -1;

        fscanf(infile, "%d %d %d", &start, &end, &len);

        if (start == -1) {
            *mistake = 1;
            printf("bad number of lines\n");
        }
        if ((!((start >= 1) && (start <= N))) || (!((end >= 1) && (end <= N)))) {
            *mistake = 1;
            printf("bad vertex\n");
        }

        if (!((len >= 0) && (len <= INT_MAX))) {
            *mistake = 1;
            printf("bad length\n");
        }

        arr[*t].length = len;
        arr[*t].start = start;
        arr[*t].end = end;
        arr[*t].val = 0;

        (*t)++;
    }

    for (int i = 0; i < *t - 1; i++) {
        for (int y = i + 1; y < *t; y++) {
            if (arr[i].length > arr[y].length) {
                s_compose temp = arr[i];
                arr[i] = arr[y];
                arr[y] = temp;
            }
        }
    }

    return arr;
}

void checking_past_edges(int *fin, int *fin1, int *g, s_compose *arr, int t, int y, int y2) {
    int u = 0;

    for (int v = 0; v <= *g; v++) {
        if (fin[v] == fin1[y2])
            u = 1;
    }

    if (u == 0) {
        (*g)++;
        fin[*g] = fin1[y2];
        marking_as_read(t, arr, fin1, y, y2, 1);
    } else {
        marking_as_read(t, arr, fin1, y, y2, -1);
    }
}

void prim_alg_implementation(s_compose *arr, int t) {
    int *fin = (int *) calloc(sizeof(int), t);
    int *fin1 = (int *) calloc(sizeof(int), t);

    int g = 0;
    int g1 = 0;

    for (int i = 0; i < t; i++) {
        if (g == 0) {

            fin[g] = arr[i].start;
            g++;

            fin[g] = arr[i].end;
            arr[i].val = 1;

        } else {
            if (arr[i].val == 0) {

                int h = 0;
                int var1 = 0;

                for (int k = 0; k <= g; k++) {
                    if (arr[i].start == fin[k]) {
                        h++;
                        var1 = 1;
                    } else if (arr[i].end == fin[k]) {
                        h++;
                    }
                }

                if (h == 1) {
                    if (var1) {
                        g++;
                        fin[g] = arr[i].end;
                    } else {
                        g++;
                        fin[g] = arr[i].start;
                    }

                    for (int y = 0; y <= g1; y++) {
                        if ((fin1[y] == arr[i].start) || (fin1[y] == arr[i].end)) {

                            if (y % 2 == 0) {
                                checking_past_edges(fin, fin1, &g, arr, t, y, y + 1);
                            } else {
                                checking_past_edges(fin, fin1, &g, arr, t, y, y - 1);
                            }

                            for (int v = y; v < t - 2; v++) {
                                fin1[v] = fin1[v + 2];
                            }
                            break;
                        }
                    }
                    arr[i].val = 1;
                } else if (h == 2) {
                    arr[i].val = -1;
                } else if (h == 0) {
                    if (g1 == 0) {
                        fin1[g1] = arr[i].start;
                        g1++;
                        fin1[g1] = arr[i].end;
                    } else {
                        g1++;
                        fin1[g1] = arr[i].start;
                        g1++;
                        fin1[g1] = arr[i].end;
                        g1--;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *infile = fopen(argv[1], "rb");

    int vert_amount = 0;
    int mistake = 0;
    s_compose *arr = sorting_edges(infile, &vert_amount, &mistake);

    if (mistake)
        return 0;

    prim_alg_implementation(arr, vert_amount);

    if (vert_amount == 1)
        printf("no spanning tree\n");
    else
        for (int i = 0; i < vert_amount; i++) {
            if (arr[i].val == 1) printf("%d %d\n", arr[i].start, arr[i].end);
        }
    return 0;
}