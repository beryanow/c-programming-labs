#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int whether_pattern_is_empty(const char *pattern) {
    if (pattern[0] == '\0') return 1;
    return 0;
}

void print_protocol(int prime_hash, int *output_line, int k, int check1) {
    if (check1 == 1)
        printf("no result");
    else {
        printf("%d ", prime_hash);
        for (int i = 0; i < k; i++) {
            printf("%d ", output_line[i]);
        }
    }
}

int comparing_every_letter(int *g1, char *argv, const char *argv1, int *position, int *g, int *check, int d) {
    int check1 = 1;
    *g1 = (int) (strlen(argv) - 1);
    int c = *position - *g1;
    for (*g = *position; *g >= c; (*g)--) {
        if (argv1[*g] != argv[*g1]) {
            check1 = 0;
        }
        (*g1)--;
    }
    if (check1 == 1) {
        *position = d;
        *check = 1;
    }
    return -1;
}

int powering(int prime, int i) {
    int result = 1;
    for (int k = 1; k <= i; k++) {
        result *= prime;
    }
    return result;
}

void recalculating_hash(int *new_hash, const char *argv, int *position, int h, int prime, int *d) {
    *new_hash = (*new_hash - (int) (argv[(*position) - h]) % 3) / prime +
                (int) argv[(*position) + 1] % 3 * powering(prime, h);
    (*position)++;
    *d = *position;
}

void calculating_hash(char *argv, const char *argv1, int *prime_hash, int *new_hash, int prime) {
    int h = (int) strlen(argv);
    for (int i = 0; i < h; i++) {
        *prime_hash += ((int) (argv[i]) % 3) * powering(prime, i);
        *new_hash += ((int) (argv1[i]) % 3) * powering(prime, i);
    }
}

int *rabin_karp_algorithm(char *str1, char *str2, int prime_hash, int new_hash, int prime, int *position, int *check1, int *f) {
    int g1; // g1 is used for going through the pattern from end to start
    int g = *position, h = g; // g is used for going through the input string part from end to start
    int check = 0, d = *position; // d is used to correct position value when a result is found
    while (!((prime_hash == new_hash) && (check == 1))) {
        if (*position > strlen(str2)) {
            *check1 = 1;
            return 0;
        }
        if (prime_hash == new_hash) {
            int k = comparing_every_letter(&g1, str1, str2, &(*position), &g, &check, d);
            if (k == -1) {
                d = *position;
                (*position)++;
            }
        } else
            recalculating_hash(&new_hash, str2, &(*position), h, prime, &d);
    }
    int *output_line = (int *) calloc(strlen(str1), sizeof(int));
    int h1 = (int) strlen(str1);
    for (int i = *position - h1 + 1; i <= *position; i++) {
        output_line[*f] = i;
        (*f)++;
    }
    return output_line;
}

int main(int argc, char *argv[]) {
    int prime = 3, prime_hash = 0, new_hash = 0;

    // checking whether pattern is valid
    if (whether_pattern_is_empty(argv[1])) {
        printf("no result");
        return 0;
    }

    calculating_hash(argv[1], argv[2], &prime_hash, &new_hash, prime);

    // creating a protocol of Rabin-Karp algorithm implementation
    int check1 = 0, f = 0;
    int position = (int) (strlen(argv[1]) - 1);
    int *output_line = rabin_karp_algorithm(argv[1], argv[2], prime_hash, new_hash, prime, &position, &check1, &f);

    // printing the protocol if found
    print_protocol(prime_hash, output_line, f, check1);

    return 0;
}
