#include <stdio.h>
#include <limits.h>

int main() {
    int N, M;
    scanf("%d", &N);
    scanf("%d", &M);
    int G[N][N];
    int A[N];
    for (int i = 0; i < N; i++) {
        A[i] = 0;
        for (int j = 0; j < N; j++) {
            G[i][j] = 0;
        }
    }
    for (int i = 0; i < M; i++) {
        int start, end, length;
        scanf("%d %d %d", &start, &end, &length);
        G[start - 1][end - 1] = length;
        G[end - 1][start - 1] = length;

    }
    int min = INT_MAX, str_num, col_num, str_num1, col_num1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((G[i][j] > 0) && (G[i][j] < min)) {
                min = G[i][j];
                str_num = i;
                col_num = j;
            }
        }
    }
    A[col_num] = 1; A[str_num] = 1;
    int am = 0;
    while (am != N - 1) {
        printf("%d %d\n", str_num + 1, col_num + 1);
        for (int i = 0; i < N; i++) {
            if (i != col_num)
                G[str_num][i] = 0;
            G[col_num][i] = 0;
        }
        min = INT_MAX;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((G[i][j] > 0) && (G[i][j] <= min) && ((A[i] == 1) &&(A[j] == 0)) | ((A[i] == 0) &&(A[j] == 1)) && (((i != str_num) | (j != col_num)))) {
                    min = G[i][j];
                    str_num1 = i;
                    col_num1 = j;
                }
            }
        }
        col_num = col_num1;
        str_num = str_num1;
        A[col_num] = 1;
        A[str_num] = 1;
        am++;
    }
    return 0;
}
