#include <stdio.h>
#include <stdlib.h>

void sift(int *a, int i, int n) {
    int l, r, k;
    int tmp;
    i++;
    while ((l = 2 * i) <= n) {
        if (l + 1 <= n) {
            r = l + 1;
        }
        else r = i;
        if ((a[i - 1] >= a[l - 1]) && (a[i - 1] >= a[r - 1])) {
            return;
        }
        if (a[l - 1] >= a[r - 1]) {
            k = l;
        }
        else k = r;
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
        sift(a, i, n);
    }
    for (i = n - 1; i > 0; i--) {
        tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        sift(a, 0, i);
    }
}

int main(int argc, char* argv[]) {
    int i, n;
    int *a;
    a = (int*)malloc(n * sizeof(int));
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