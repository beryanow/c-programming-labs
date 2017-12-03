#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printing_decimal_part(char *output_line) {
    for (int f = 0; f < strlen(output_line); f++)
        printf("%c", output_line[f]);
}

void deleting_useless_zeros(char **output_line) {
    for (int f = strlen(*output_line) - 1; f >= 0; f--) {
        if ((*output_line)[f] == '0')
            (*output_line)[f] = '\0';
        else break;
    }
}

void add_A_to_F(int x, char **output, int *s) {
    if ((x >= 10) && (x <= 15)) {
        (*output)[*s] = (char) ((int) ('A') + x - 10);
        (*s)++;
    } else {
        (*output)[(*s)] = (char) (x + (int) '0');
        (*s)++;
    }
}

void print_A_to_F(int x) {
    if ((x >= 10) && (x <= 15)) {
        printf("%c", (char) ((int) ('A') + x - 10));
    } else printf("%d", x);
}

int to_new(char x) {
    if ((x >= 'A') && (x <= 'Z')) return x - 'A' + 10;
    else return x - '0';
}

int check(char *x, int old_base, int new_base) {
    int denied_symb; // denied_symb - показатель, существуют ли в исходном числе недопустимые символы для той системы счисления (1 - существуют, 0 - не существуют)
    for (int i = 0; i < strlen(x); i++) {
        if (x[i] == '.') continue;
        if ((old_base < 11) && (x[i] - '0' >= old_base)) {
            denied_symb = 1;
        }
        if ((old_base > 10) && !(((x[i] >= 'A') && (x[i] <= 'Z')) | ((x[i] >= '0') && (x[i] <= '9')))) {
            denied_symb = 1;
        }
    }
    if ((old_base > 16) | (old_base < 2) | (new_base > 16) | (new_base < 2) | (denied_symb == 1)) {
        printf("Bad input");
        return 1;
    } else return 0;
}

int transfer_int(char *x, int old_base) {
    int new, int_part, dot_number = -1;
    new = to_new(x[0]);
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            int_part = new; // int_part - целая часть числа
            dot_number = i +
                         1; // dot_number - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        } else {
            new = new * old_base + to_new(x[i + 1]);
        }
    }
    if (dot_number == -1) {
        int_part = new;
    }
    return int_part;
}

float transfer_dec(char *x, int old_base) {
    int dot_number = -1;
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            dot_number = i +
                         1; // dot_number - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        }
    }
    float dec_part;
    if (dot_number != -1) {
        float old_base1 = old_base, new1;
        new1 = to_new(x[strlen(x) - 1]) / old_base1;
        for (int i = strlen(x) - 1; i > (dot_number + 1); i--) {
            new1 = (new1 + to_new(x[i - 1])) / old_base1;
        }
        dec_part = new1; // dec_part - дробная часть числа
        return dec_part;
    }
}

void put_to_screen(char *x, int int_part, int new_base, float dec_part) {
    int a[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int m = 0, not_digit = 0, dot_number = -1;;
    float new_base1 = new_base;
    while (int_part / new_base1 > 0) { // перевод целой части числа в новую систему счисления
        if (new_base > 10 && int_part % new_base >= 10) {
            a[m] = int_part % new_base;
            not_digit = 1; // NotDigit - показатель, существуют ли в записи числа "цифры", превосходящие 9 (1 - существуют, 0 - не существуют)
        } else {
            a[m] = int_part % new_base;
        }
        int_part /= new_base;
        m++;
    }
    for (int i = m - 1; i >= 0; i--) { // вывод целой части нового числа
        if (not_digit == 0) {
            printf("%d", a[i]);
        } else print_A_to_F(a[i]);
    }
    for (int i = 0; i < strlen(x) - 1; i++) { // перевод целой части числа в промежуточную систему счисления
        if (x[i + 1] == '.') {
            dot_number = i + 1; // DotNumber - номер позиции точки, если у числа существует дробная часть (равен -1, если дробной части нет)
            break;
        }
    }
    char *output_line = (char *) calloc(12, sizeof(char));
    int s = 0;
    if (dot_number != -1) { // вывод дробной части нового числа
        printf(".");
        for (int i = 0; i < 12; i++) {
            int m = (int) (dec_part * new_base);
            if (new_base <= 10) {
                output_line[s] = (char)(m + (int) '0');
                s++;
            } else add_A_to_F(m, &output_line, &s);
            dec_part = dec_part * new_base - m;
        }
        deleting_useless_zeros(&output_line);
        printing_decimal_part(output_line);
    }
}

int main(int argc, char *argv[]) {
    int old_base = atoi(argv[1]), new_base = atoi(argv[2]), int_part;
    float dec_part;
    if (check(argv[3], old_base, new_base) == 0) {
        int_part = transfer_int(argv[3], old_base);
        dec_part = transfer_dec(argv[3], old_base);
        put_to_screen(argv[3], int_part, new_base, dec_part);
    }
    return 0;
}