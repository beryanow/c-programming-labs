#include <stdio.h>
#include <stdlib.h>

struct node {
    int *len;
    int last;
    int color; // 0 - white, 1 - grey, 2 - black
};

void print_result(int h, int d, int *final) {
    if (h != 1) {
        for (int i = 0; i < d; i++) {
            printf("%d ", final[i]);
        }
    } else printf("ERROR: Impossible to sort.");
}

int check_values(int start, int n, int end) {
    if ((!((start >= 1) && (start <= n))) || (!((end >= 1) && (end <= n)))) {
        printf("ERROR: Bad vertex.");
        return 0;
    }
    if ((start == 0) || (end == 0)) {
        printf("ERROR: Bad number of lines.");
        return 0;
    }
    return 1;
}

int check_amounts(int n, int m) {
    if (!((n >= 0) && (n <= 1000))) {
        printf("ERROR: Bad number of vertices.");
        return 0;
    }
    if (!((m >= 0) && (m <= n * (n + 1) / 2))) {
        printf("ERROR: Bad number of edges.");
        return 0;
    }
    return 1;
}

void topological_sorting(int *i, struct node *arr, int *final, int *h, int *d) {
    if (arr[*i].color == 1) {
        *h = 1;
    } else {
        arr[*i].color = 1;
        for (int b = 0; b <= arr[*i].last; b++) {
            int temp = *i;
            *i = arr[*i].len[b];
            topological_sorting(i, arr, final, h, d);
            *i = temp;
        }
        arr[*i].color = 2;
        if (*h != 1) {
            for (int x = *d; x > 0; x--) {
                final[x] = final[x - 1];
            }
            final[0] = *i;
            (*d)++;
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *infile = fopen(argv[1], "rb");

    int n, m;
    fscanf(infile, "%d %d", &n, &m);

    if (check_amounts(n, m) == 0)
        return 0;

    struct node *arr = (struct node *) malloc(sizeof(struct node) * n + 1);

    for (int i = 0; i <= n; i++) {
        arr[i].color = 0;
        arr[i].last = -1;
        arr[i].len = (int *) calloc(sizeof(int), n + 1);
    }

    int start = 0, end = 0;
    for (int i = 2; i < m + 2; i++) {
        fscanf(infile, "%d %d", &start, &end);

        if (check_values(start, n, end) == 0)
            return 0;

        while (arr[start].len[arr[start].last + 1] != 0)
            arr[start].last++;

        arr[start].len[arr[start].last + 1] = end;
        arr[start].last++;

    }

    int *final = (int *) calloc(sizeof(int), n);
    int i = start, check = 0, d = 0;

    topological_sorting(&i, arr, final, &check, &d);

    print_result(check, d, final);

    return 0;
}