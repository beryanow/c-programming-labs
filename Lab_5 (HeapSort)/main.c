#include <stdio.h>
#include <stdlib.h>

void showing_output_line(int g, int *arr2) {
    for (int i = 1; i <= g; i++) printf("%d ", arr2[i]);
}

void deleting_element_from_main_node(int *n, int **arr) {
    for (int i = 1; i <= *n - 1; i++) {
        (*arr)[i] = (*arr)[i + 1];
    }
    (*n)--;
}

void adding_element_to_output_line(int *m, int **arr2, int max) {
    (*m)++;
    (*arr2)[1] = max;
}

void shifting_values_in_output_line_to_the_left_by_one(int m, int **arr2) {
    for (int i = m; i >= 1; i--) {
        (*arr2)[i + 1] = (*arr2)[i];
    }
}

void changing_max_value_with_main_node(int **arr, int max, int max_num) {
    int temp = (*arr)[1];
    (*arr)[1] = max;
    (*arr)[max_num] = temp;
}

void finding_max_element(int *arr, int n, int *max, int *max_num) {
    for (int i = 1; i <= n; i++) {
        if (arr[i] >= *max) {
            *max = arr[i];
            *max_num = i;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    int *arr = (int *) calloc(n, sizeof(int));
    int *arr2 = (int *) calloc(n, sizeof(int));
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }
    int g = n, m = 0; // m is used for following how many elements are already in output line
    while (n != 0) {
        int max = arr[1];
        int max_num = 1;
        finding_max_element(arr, n, &max, &max_num);
        changing_max_value_with_main_node(&arr, max, max_num);
        shifting_values_in_output_line_to_the_left_by_one(m, &arr2);
        adding_element_to_output_line(&m, &arr2, max);
        deleting_element_from_main_node(&n, &arr);
    }
    showing_output_line(g, arr2);
    return 0;
}