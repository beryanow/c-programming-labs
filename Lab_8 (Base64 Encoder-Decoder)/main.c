#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help() {
    printf("USAGE:\n [for decoding]: -d -i inputfilename outputfilename \n "
                   "(-i is not necessary and used if your \"inputfilename\" file consists of none-base64 alphabet symbols)\n "
                   "[for encoding]: -e -f fcount inputfilename outputfilename \n "
                   "(-f fcount is not necessary and used if you want to place *new string* symbol every fcount symbols)");
}

void converting_to_original_symbols(int break_num, unsigned char *original_symbols, const char *base64_symbols, int *m) {
    if (break_num == 2) {
        original_symbols[0] = (unsigned char) (((base64_symbols[0] & 63) << 2) |
                                      ((base64_symbols[1] & 48) >> 4));
        *m = 1;
    } else if (break_num == 3) {
        original_symbols[0] = (unsigned char) (((base64_symbols[0] & 63) << 2) |
                                      ((base64_symbols[1] & 48) >> 4));
        original_symbols[1] = (unsigned char) (((base64_symbols[1] & 15) << 4) |
                                      ((base64_symbols)[2] & 60) >> 2);
        *m = 2;
    } else {
        original_symbols[0] = (unsigned char) (((base64_symbols[0] & 63) << 2) |
                                      ((base64_symbols[1] & 48) >> 4));
        original_symbols[1] = (unsigned char) (((base64_symbols[1] & 15) << 4) |
                                      ((base64_symbols)[2] & 60) >> 2);
        original_symbols[2] = (unsigned char) (((base64_symbols[2] & 3) << 6) | base64_symbols[3]);
        *m = 3;
    }
}

void converting_to_base64_symbols(char *base64_symbols, const char base64_table[64], const unsigned char *original_symbols, int count) {
    base64_symbols[0] = base64_table[((int) original_symbols[0] & 252) >> 2];
    base64_symbols[1] = base64_table[((((int) original_symbols[0] & 3) << 4) |
                                      (((int) original_symbols[1] & 240) >> 4))];
    if (count == 2) {
        base64_symbols[2] = base64_table[((((int) original_symbols[1] & 15) << 2) |
                                          (((int) original_symbols[2] & 192) >> 6))];
        base64_symbols[3] = '=';
    } else if (count == 1) {
        base64_symbols[2] = '=';
        base64_symbols[3] = '=';
    } else {
        base64_symbols[2] = base64_table[((((int) original_symbols[1] & 15) << 2) |
                                          (((int) original_symbols[2] & 192) >> 6))];
        base64_symbols[3] = base64_table[((int) original_symbols[2] & 63)];
    }
}

void finding_commands(int *i, int argc, char *argv[], char input_string[30], int *e_cmd, int *d_cmd, int *i_cmd, int *f_cmd, int *f_cmd_num, int *tr1) {
    for (*i = 1; *i < argc - 2; (*i)++) {
        strcpy(input_string, argv[*i]);
        if (input_string[0] == '-') {
            switch (input_string[1]) {
                case 'e':
                    *e_cmd = 1;
                    break;
                case 'd':
                    *d_cmd = 1;
                    break;
                case 'i':
                    *i_cmd = 1;
                    break;
                case 'f':
                    *f_cmd = 1;
                    (*i)++;
                    strcpy(input_string, argv[*i]);
                    *f_cmd_num = atoi(input_string);
                    if (argc == 5) {
                        print_help();
                        *tr1 = 1;
                    }
                    break;
                default:
                    print_help();
                    *tr1 = 1;
                    break;
            }
        } else {
            print_help();
            *tr1 = 1;
        }
    }
}

void decoding(int *j, long int file_size, int i_cmd, const unsigned char *original_file, const char base64_table[64], char *base64_symbols, unsigned char *original_symbols, FILE *outfile) {
    while (*j < file_size - 1) {
        for (int p = 0; p < 4; p++) {
            if (i_cmd == 1) {
                int tr = 0;
                for (int z = 0; z < 64; z++) {
                    if (original_file[*j + p] == '=') {
                        tr = 1;
                        break;
                    } else if (base64_table[z] == original_file[*j + p]) {
                        tr = 1;
                        break;
                    }
                }
                if (tr == 0) {
                    (*j)++;
                    p--;
                } else
                    base64_symbols[p] = original_file[*j + p];
            } else base64_symbols[p] = original_file[*j + p];
        }
        int m = 0;
        int break_num = 0;
        for (int p = 0; p < 4; p++) {
            if (base64_symbols[p] == '=') {
                if (break_num == 0)
                    break_num = p;
            } else
                for (int z = 0; z < 64; z++)
                    if (base64_table[z] == base64_symbols[p]) {
                        base64_symbols[p] = (char) z;
                        break;
                    }
        }
        converting_to_original_symbols(break_num, original_symbols, base64_symbols, &m);
        for (int i = 0; i < m; i++) {
            fputc(original_symbols[i], outfile);
        }
        *j += 4;
    }
}

void encoding(int *j, long int file_size, unsigned char *original_symbols, char *base64_symbols, const char base64_table[64], const unsigned char *original_file, FILE *outfile, int f_cmd, int f_cmd_num) {
    int cg = 0;
    while (*j < file_size - 1) {
        int count = 0;
        for (int p = 0; p < 3; p++) {
            original_symbols[p] = '\0';
        }
        for (int p = 0; p < 3; p++) {
            if (*j + p < file_size) {
                original_symbols[p] = original_file[*j + p];
                count++;
            }
        }
        converting_to_base64_symbols(base64_symbols, base64_table, original_symbols, count);
        for (int p = 0; p < 4; p++) {
            fputc(base64_symbols[p], outfile);
            if (f_cmd == 1) {
                cg++;
                if (cg == f_cmd_num) {
                    fputc('\n', outfile);
                    cg = 0;
                }
            }
        }
        *j += 3;
    }
}

int main(int argc, char *argv[]) {
    if ((argc > 3) && (argc < 7)) {
        int i;
        char input_string[30];
        int e_cmd = 0, d_cmd = 0, i_cmd = 0, f_cmd = 0, f_cmd_num = -1, tr1 = 0;
        finding_commands(&i, argc, argv, input_string, &e_cmd, &d_cmd, &i_cmd, &f_cmd, &f_cmd_num, &tr1);
        if (tr1 == 0) {
            char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            FILE *infile = fopen(argv[i], "rb");
            FILE *outfile = fopen(argv[i + 1], "wb");
            fseek(infile, 0, SEEK_END);
            long int file_size;
            file_size = ftell(infile);
            rewind(infile);
            unsigned char *original_file;
            original_file = (unsigned char *) malloc(sizeof(unsigned char) * file_size);
            fread(original_file, sizeof(unsigned char), (size_t) file_size, infile);
            unsigned char *original_symbols = (unsigned char *) malloc(3 * sizeof(char));
            char *base64_symbols = (char *) malloc(4 * sizeof(char));
            int j = 0;
            if (e_cmd == 1) {
                encoding(&j, file_size, original_symbols, base64_symbols, base64_table, original_file, outfile, f_cmd,
                         f_cmd_num);
            } else if (d_cmd == 1) {
                decoding(&j, file_size, i_cmd, original_file, base64_table, base64_symbols, original_symbols, outfile);
            }
        }
    } else print_help();
    return 0;
}