#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void finding_min (int i, int p, const char* x, char* min, int* num_min) {
    for (int k = i + 1; k < p; k++) {
        if ((x[k] < (char) min) && (x[k] > x[i])) {
            *min = x[k];
            *num_min = k;
        }
    }
}

int check_for_bad_input (char* x, const char* y) {
    if ((*x == '\0') | (*y == '\0'))
        return 1;
    int c = 0;
    int table[256];
    for (int i = 0; i < 256; i++)
        table[i] = 0;
    for (int k = 0; k <= strlen(x) - 1; k++) {
        table[(int)x[k]]++;
        if (((table[(int)x[k]]) > 1) | (!(((x[k] >= '0') && (x[k] <= '9'))))) return 1;
    }
    return c;
}

int check_for_available_shifts (char* x) {
    int c = 0;
    for (int k = 0; k < strlen(x) - 1; k++) {
        if (x[k] < x[k + 1]) c = 1;
    }
    return c;
}

char* swap_digits (int i, int num_min, char* x) {
    char tmp = x[i];
    x[i] = x[num_min];
    x[num_min] = tmp;
    return x;
}

char* ascending_sorting_of_tale (int q, char* x) {
    int p = (int) strlen(x);
    for (int z = q; z < p - 1; z++) {
        for (int c = z + 1; c < p; c++) {
            if (x[z] > x[c]) {
                x = swap_digits(z, c, x);
            }
        }
    }
    return x;
}

void make_some_shifts (char* x, char* y) {
    int p = (int) strlen(x);
    int num_min, q, m = 0, i = p - 2;
    while (i != -1) {
        if (x[i] < x[i + 1]) {
            q = i + 1;
            char min = x[i + 1];
            num_min = i + 1;
            finding_min(i, p, x, &min, &num_min);
            x = swap_digits(i, num_min, x);
            x = ascending_sorting_of_tale(q, x);
            printf("%s ", x);
            i = p - 2;
            m++;
            if (m > atoi(y) - 1) break;
        }
        else i--;
    }
}

int main(int argc, char* argv[]) {
    if (check_for_bad_input(argv[1], argv[2])) printf("bad input");
    else
    if (check_for_available_shifts(argv[1])) {
        // making shifts using Dijkstra algorithm
        make_some_shifts(argv[1], argv[2]);
    }

    return 0;
}
