#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_protocol(int *output_protocol, int protocol_size) {
    for (int i = 0; i < protocol_size; i++)
        printf("%d ", output_protocol[i]);
}

int empty_pattern(const char *pattern) {
    if (pattern[0] == '\0') return 1;
    return 0;
}

int *make_output_line(char *pattern, char **input_string, const int *shifts_table, int *check, int *m) {
    int pattern_length = (int) strlen(pattern);
    int input_string_length = (int) strlen(*input_string);
    int position = pattern_length, pattern_last_element = pattern_length - 1;
    int c = 0; // amount of same letters found before one that doesn't suit
    int *output_line = (int *) calloc(strlen(*input_string), sizeof(int));
    *m = 0; //amount of numbers in protocol

    while (pattern_last_element != -1) {
        if (position > input_string_length) {
            *check = 1;
            return output_line;
        }
        if ((*input_string)[position - 1] == pattern[pattern_last_element]) {
            output_line[*m] = position;
            (*m)++;
            pattern_last_element--;
            position--;
            c++;
        } else {
            output_line[*m] = position;
            (*m)++;
            position += shifts_table[(unsigned int) (*input_string)[position - 1]] + c;
            pattern_last_element += c;
            c = 0;
        }
    }
    return output_line;
}

void create_shifts_table(int *shifts_table, char *argv) {
    int m = 1, i;
    int g = (int) strlen(argv);
    for (i = 0; i < 256; i++) {
        shifts_table[i] = g;
    }
    for (i = g - 2; i >= 0; i--) {
        if (shifts_table[i] == g)
            shifts_table[(unsigned int) argv[i]] = m;
        m++;
    }
}

int *boyer_moore_algorithm(char *str1, char *str2, const int *check_if_result_exists, const int *m) {
    int *shifts_table[256];
    create_shifts_table(&shifts_table, str1);
    return make_output_line(str1, &str2, &shifts_table, &(*check_if_result_exists), &(*m));
}

void print_protocol(int check_if_result_exists, int *boyer_moore_output_protocol, int m) {
    if (check_if_result_exists == 1)
        printf("no result");
    else
        show_protocol(boyer_moore_output_protocol, m);
}

int main(int argc, char *argv[]) {

    // checking whether pattern is valid
    if (empty_pattern(argv[1])) {
        printf("no result");
        return 0;
    }

    // creating a protocol of Boyer-Moore algorithm implementation
    int check_if_result_exists = 0, m;
    int *output_protocol = boyer_moore_algorithm(argv[1], argv[2], &check_if_result_exists, &m);

    // printing the protocol if found
    print_protocol(check_if_result_exists, output_protocol, m);

    return 0;
}
