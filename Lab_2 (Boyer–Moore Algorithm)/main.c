#include <stdio.h>
#include <string.h>

char* filling_in_table_for_shifting_values (char* pattern_string, int* table_for_shifting_values) {
    int m = 0;
    for (int i = 0; i < 256; i++) {
        table_for_shifting_values[i] = 0;
    }
    int x = strlen(pattern_string);
    for (int i = strlen(pattern_string) - 2 ; i > 0; i--) {
        m++;
        if (table_for_shifting_values[(unsigned int)pattern_string[i]] == 0) {
            table_for_shifting_values[(unsigned int)pattern_string[i]] = m;
        }
    }
    if (table_for_shifting_values[(unsigned int)pattern_string[x - 1]] == 0) {
        table_for_shifting_values[(unsigned int)pattern_string[x - 1]] = x;
    }
    return pattern_string;
}

int putting_values_to_screen_using_boyer_moore_algorithm (char* pattern_string, char* combined_string, int* table_for_shifting_values) {
    int position = strlen(pattern_string);
    int z = strlen(pattern_string) - 1;
    int z1 = strlen(pattern_string);
    while (z != -1) {
        if (position > strlen(combined_string)) return -1;
        if (pattern_string[z] == combined_string[position]) {
            z--;
            position--;
        }
        else {
            if (table_for_shifting_values[(unsigned int)combined_string[position]] == 0) position += 2 * z1 - z - 1;
            else position += table_for_shifting_values[(unsigned int)combined_string[position]];
            z = z1 - 1;
        }
    }
    return position + 1;
}

int main(int argc, char* argv[]) {
    int table_for_shifting_values[256];
    char* pattern_string = filling_in_table_for_shifting_values(argv[1], table_for_shifting_values);
    printf("%d", putting_values_to_screen_using_boyer_moore_algorithm(pattern_string, argv[2], table_for_shifting_values));
    return 0;
}


