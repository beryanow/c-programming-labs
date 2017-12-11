#include <stdio.h>
#include <stdlib.h>

void sifting(int *a, int i, int n) {
    int left, right, k;
    int tmp;
    i++;
    while ((left = 2 * i) <= n) {
        if (left + 1 <= n) {
            right = left + 1;
        } else right = i;
        if ((a[i - 1] >= a[left - 1]) && (a[i - 1] >= a[right - 1])) {
            return;
        }
        if (a[left - 1] >= a[right - 1]) {
            k = left;
        } else k = right;
        tmp = a[i - 1];
        a[i - 1] = a[k - 1];
        a[k - 1] = tmp;
        i = k;
    }
}

void heap_sort(int *a, int n) {
    int i;
    int tmp;
    for (i = n / 2; i >= 0; i--) {
        sifting(a, i, n);
    }
    for (i = n - 1; i > 0; i--) {
        tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        sifting(a, 0, i);
    }
}

int main() {
    int i, n;
    int *a;
    a = (int *) malloc(n * sizeof(int));
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    heap_sort(a, n);
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    return 0;
}
