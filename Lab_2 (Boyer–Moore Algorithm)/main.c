#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void showing_protocol(int *output_line, int m) {
    for (int i = 0; i < m; i++)
        printf("%d ", output_line[i]);
}

int whether_pattern_is_empty(char *pattern) {
    if (pattern[0] == '\0') return 1;
    return 0;
}

int *making_output_line(char *pattern, char **string_to_search, int *shifts_table, int *check, int *m) {
    int pattern_length = strlen(pattern);
    int string_to_search_length = strlen(*string_to_search);
    int position = pattern_length, pattern_last_element = pattern_length - 1;
    int c = 0; // amount of same letters found before one that doesn't suit
    int *output_line = (int *) calloc(strlen(*string_to_search), sizeof(int));
    *m = 0; //amount of numbers in protocol
    while (pattern_last_element != -1) {
        if (position > string_to_search_length) {
            *check = 1;
            return output_line;
        }
        if ((*string_to_search)[position - 1] == pattern[pattern_last_element]) {
            output_line[*m] = position;
            (*m)++;
            pattern_last_element--;
            position--;
            c++;
        } else {
            output_line[*m] = position;
            (*m)++;
            position += shifts_table[(unsigned int) (*string_to_search)[position - 1]] + c;
            pattern_last_element += c;
            c = 0;
        }
    }
    return output_line;
}

void create_shifts_table(int *shifts_table, char *argv) {
    int m = 1, i;
    int g = strlen(argv);
    for (i = 0; i < 256; i++) {
        shifts_table[i] = g;
    }
    for (i = g - 2; i >= 0; i--) {
        if (shifts_table[i] == g) shifts_table[(unsigned int) argv[i]] = m;
        m++;
    }
}

int main(int argc, char *argv[]) {
    /* checking whether pattern is valid */
    if (whether_pattern_is_empty(argv[1])) {
        printf("no result");
        return 0;
    }
    /* making shifts table */
    int shifts_table[256];
    create_shifts_table(&shifts_table, argv[1]);
    /* creating output line consisting of protocol values */
    int check_if_result_exists = 0, m;
    int *output_line = making_output_line(argv[1], &argv[2], &shifts_table, &check_if_result_exists, &m);
    /* checking whether to put protocol to screen */
    if (check_if_result_exists == 1)
        printf("no result");
    else
        showing_protocol(output_line, m);
    return 0;
}