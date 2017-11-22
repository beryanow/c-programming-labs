#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int exponentiation (int a, int b) {
    int c = 1;
    for (int i = 0; i < b; i++) {
        c *= a;
    }
    return c;
}

int recalс_hash (int new_hash, char* y, int m, int i, int x) {
    return (new_hash - ((unsigned int)y[m] % 3)) / 3 + ((unsigned int)y[i + x - 1] % 3) * exponentiation(3, x - 1);
}

int check_for_overlap (int prime_hash, int new_hash, char* x1, char* y1, int x, int i) {
    int p = 1, a = 0;
    if (prime_hash == new_hash) {
        int a1;
        for (a1 = i; a1 < i + x; a1++) {
            if (x1[a] != y1[a1]) p = 0;
            a++;
        }
        if (p == 1) return a1;
    }
    return -1;
}

int calc_hash (char* x) {
    int hash = 0, p = 0;
    for (int i = 0; i < strlen(x); i++) {
        hash += ((unsigned int)(x[i]) % 3) * exponentiation(3, p);
        p++;
    }
    return hash;
}

int main(int argc, char* argv[]) {
    int x = strlen(argv[1]), x2 = strlen(argv[2]);
    char* part_for_new_hash = (char*)malloc(x * sizeof(char));
    for (int i = 0; i < x; i++) {
        part_for_new_hash[i] = argv[2][i];
    }
    int prime_hash = calc_hash(argv[1]), new_hash = calc_hash(part_for_new_hash), i = 0;
    printf("%d ", prime_hash);
    if (check_for_overlap(prime_hash, new_hash, argv[1], argv[2], x, i) != -1) {
        printf("%d ", check_for_overlap(prime_hash, new_hash, argv[1], argv[2], x, i) - x);
        return 0;
    }
    int m = 0;
    for (int i = 1; i <= x2 - x + 1; i++) {
        new_hash = recalс_hash(new_hash, argv[2], m, i ,x);
        if (check_for_overlap(prime_hash, new_hash, argv[1], argv[2], x, i) != -1) {
            printf("%d ", check_for_overlap(prime_hash, new_hash, argv[1], argv[2], x, i) - x);
            return 0;
        }
        m++;
    }
    printf("-1");
    return 0;
}