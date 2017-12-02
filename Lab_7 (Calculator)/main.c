//to_do: make a function to check whether the input line is valid

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void which_sign(char f, int* result, int result2, int result1) {
    switch (f) {
        case '+':
            *result = result2 + result1;
            break;
        case '-':
            *result = result2 - result1;
            break;
        case '*':
            *result = result2 * result1;
            break;
        case '/':
            *result = result2 / result1;
            break;
    }
}

void displacing_to_the_right_by_one(int k, char** output) {
    for (int x = strlen(*output); x > k; x--) {
        (*output)[x] = (*output)[x - 1];
    }
}

void displacing_to_the_left_by_one (int k, char** output) {
    for (int x = k; x < strlen(*output) - 1; x++) {
        (*output)[x] = (*output)[x + 1];
    }
}

void displacing_to_the_left_by_two (int k, char** output) {
    for (int x = k; x < strlen(*output) - 2; x++) {
        (*output)[x] = (*output)[x + 2];
    }
}


int new_number_to_output (int* result, char** output, int k) {
    while (*result != 0) {
        displacing_to_the_right_by_one(k, output);
        (*output)[k] = (char)((*result) % 10 + '0');
        (*result) /= 10;
    }
}

void calc_string_value (char** output, int* k, int* result) {
    int h = 1;
    while ((*output)[*k] != '(') {
        *result += ((*output)[*k] - '0') * h;
        h *= 10;
        displacing_to_the_left_by_one(*k, output);
        (*output)[strlen(*output) - 1] = '\0';
        (*k)--;
    }
}

void numbers_into_brackets (char* x, char** output, int* m, int n, int i) {
    (*output)[*m] = '(';
    (*m)++;
    for (int z = n; z < i; z++) {
        (*output)[*m] = x[z];
        (*m)++;
    }
    (*output)[*m] = ')';
    (*m)++;
}

int main(int argc, char *argv[]) {
    char *output = (char *) calloc(strlen(argv[1]), sizeof(char));
    char *stack = (char *) calloc(strlen(argv[1]), sizeof(char));
    int m = 0; //index of output line
    int k = 0; //index of stack line
    int i = 0; //index of input line

    /* creating Reverse Polish notation */
    while (i != strlen(argv[1])) {
        if ((argv[1][i] >= '0') && (argv[1][i] <= '9')) {
            int n = i;
            while ((argv[1][i] >= '0') && (argv[1][i] <= '9')) {
                i++;
            }
            numbers_into_brackets(argv[1], &output, &m, n, i);
        } else if ((argv[1][i] == '*') | (argv[1][i] == '/')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/')) {
                    output[m] = stack[k - 1];
                    stack[k - 1] = argv[1][i];
                    m++;
                } else {
                    stack[k] = argv[1][i];
                    k++;
                    break;
                }
                k--;
            }
            i++;
        } else if ((argv[1][i] == '+') | (argv[1][i] == '-')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/') | (stack[k - 1] == '+') | (stack[k - 1] == '-')) {
                    output[m] = stack[k - 1];
                    stack[k - 1] = argv[1][i];
                    m++;
                } else {
                    stack[k] = argv[1][i];
                    k++;
                    break;
                }
                k--;
            }
            i++;
        } else if (argv[1][i] == '(') {
            stack[k] = argv[1][i];
            k++;
            i++;
        } else if (argv[1][i] == ')') {
            int c = k - 1, z;
            while (stack[k] != '(') k--;
            for (z = c; z > k; z--) {
                output[m] = stack[z];
                m++;
                stack[z] = '\0';
            }
            stack[z] = '\0';
            k = z;
            i++;
        } else i++;
    }
    for (int i = k - 1; i >= 0; i--) {
        output[m] = stack[i];
        stack[i] = '\0';
        k--;
        m++;
    }
    /* end of creating Reverse Polish notation */

    /* result calculation */
    int result = 0;
    k = 0;
    int final_result;
    while (output[strlen(output) - 1] != ')') {
        while (!((output[k] == '+') | (output[k] == '-') | (output[k] == '*') | (output[k] == '/'))) k++;
        char f = output[k];
        displacing_to_the_left_by_one(k, &output);
        output[strlen(output) - 1] = '\0';
        k -= 2;
        int result1 = 0, result2 = 0;
        calc_string_value(&output, &k, &result1);
        displacing_to_the_left_by_two(k, &output);
        for (int g = 0; g <= 1; g++) {
            output[strlen(output) - 1] = '\0';
            k--;
        }
        calc_string_value(&output, &k, &result2);
        which_sign(f, &result, result2, result1);
        final_result = result;
        k++;
        new_number_to_output(&result, &output, k);
    }
    printf("%d\n", final_result);
    /* end of result calculation */

    return 0;
}
