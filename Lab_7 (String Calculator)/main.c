#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int valid_string (char* x) {
    int left_brackets = 0, right_brackets = 0;
    for (int i = 0; i < strlen(x); i++) {
        if (!((x[i] == '+') | (x[i] == '-') | (x[i] == '*') | (x[i] == '/') | (x[i] == ' ') | ((x[i] >= '0') && (x[i] <= '9')) | (x[i] == ')') | (x[i] == '('))) {
            return 0;
        }
        if (x[i] == '(') left_brackets++;
        else if (x[i] == ')') right_brackets++;
        if (right_brackets > left_brackets) return 0;
    }
    if (right_brackets != left_brackets) return 0;
    return 1;
}

int which_sign(char f, int* result, int result2, int result1) {
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
            if (result1 != 0) *result = result2 / result1;
            else return 1;
            break;
    }
    return 0;
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


void new_number_to_output (int* result, char** output, int k) {
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

char* make_polish_notation (char* str1, int* wrong1) {
    char *output = (char *) calloc(strlen(str1), sizeof(char));
    char *stack = (char *) calloc(strlen(str1), sizeof(char));
    int m = 0; //index of output line
    int k = 0; //index of stack line
    int i = 0; //index of input line
    while (i != strlen(str1)) {
        if (str1[i] == '.') {
            *wrong1 = 1;
            return output;
        }
        if ((str1[i] >= '0') && (str1[i] <= '9')) {
            int n = i;
            while ((str1[i] >= '0') && (str1[i] <= '9')) {
                i++;
            }
            numbers_into_brackets(str1, &output, &m, n, i);
        } else if ((str1[i] == '*') | (str1[i] == '/')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/')) {
                    output[m] = stack[k - 1];
                    output[m + 1] = '\0';
                    stack[k - 1] = str1[i];
                    m++;
                } else {
                    stack[k] = str1[i];
                    k++;
                    break;
                }
                k--;
            }
            i++;
        } else if ((str1[i] == '+') | (str1[i] == '-')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/') | (stack[k - 1] == '+') | (stack[k - 1] == '-')) {
                    output[m] = stack[k - 1];
                    output[m + 1] = '\0';
                    stack[k - 1] = str1[i];
                    m++;
                } else {
                    stack[k] = str1[i];
                    k++;
                    break;
                }
                k--;
            }
            i++;
        } else if (str1[i] == '(') {
            stack[k] = str1[i];
            k++;
            i++;
        } else if (str1[i] == ')') {
            if (stack[k - 1] == '(') {
                *wrong1 = 1;
                return output;
            }
            int c = k - 1, z;
            while (stack[k] != '(') k--;
            for (z = c; z > k; z--) {
                output[m] = stack[z];
                output[m + 1] = '\0';
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
        output[m + 1] = '\0';
        stack[i] = '\0';
        k--;
        m++;
    }
    return output;
}

int result_calculation (char* output, int* wrong2) {
    int result = 0;
    int k = 0;
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
        if (which_sign(f, &result, result2, result1) == 1) {
            *wrong2 = 1;
            return final_result;
        };
        final_result = result;
        k++;
        new_number_to_output(&result, &output, k);
    }
    return final_result;
}

int main(int argc, char *argv[]) {
    int wrong1 = 0, wrong2 = 0;

    // checking if symbols set in input string is correct
    if (valid_string(argv[1]) == 0) {
        printf("syntax error");
        return 0;
    }

    // creating Reverse Polish notation
    char* output = make_polish_notation(argv[1], &wrong1);

    // checking if brackets sequence is correct
    if (wrong1 == 1) {
        printf("syntax error");
        return 0;
    }

    // calculating result
    int final_result = result_calculation(output, &wrong2);

    // checking if division by zero exists
    if (wrong2 == 1) {
        printf("division be zero");
        return 0;
    }

    printf("%d\n", final_result);

    return 0;
}
