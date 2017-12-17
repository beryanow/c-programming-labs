#include <stdio.h>
#include <stdlib.h>

int *qsort_implementation(int *arr, int size) {
    int start = 0, end = size - 1;
    int middle = arr[size / 2];
    while (start <= end) {
        while (arr[start] < middle) {
            start++;
        }
        while (arr[end] > middle) {
            end--;
        }
        if (start <= end) {
            int tmp = arr[start];
            arr[start] = arr[end];
            arr[end] = tmp;
            start++;
            end--;
        }
    }

    if (end > 0) {
        qsort_implementation(arr, end + 1);
    }

    if (start < size) {
        qsort_implementation(&arr[start], size - start);
    }

    return arr;
}

void print_sorted(int n, int *table) {
    for (int i = 0; i < n; i++) {
        printf("%d ", table[i]);
    }
}

int main() {
    int n, num;
    scanf("%d", &n);
    int *table = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &num);
        table[i] = num;
    }

    // making QuickSort
    table = qsort_implementation(table, n);

    // printing line sorted
    print_sorted(n, table);
    return 0;
}
