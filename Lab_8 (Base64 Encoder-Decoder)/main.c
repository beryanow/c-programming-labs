#include <stdio.h>
#include <stdlib.h>

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

void converting_to_base64_symbols(char *base64_symbols, const unsigned char *original_symbols, int valid_bytes) {
    char base64_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    base64_symbols[0] = base64_table[((int) original_symbols[0] & 252) >> 2];
    base64_symbols[1] = base64_table[((((int) original_symbols[0] & 3) << 4) |
                                      (((int) original_symbols[1] & 240) >> 4))];
    if (valid_bytes == 2) {
        base64_symbols[2] = base64_table[((((int) original_symbols[1] & 15) << 2) |
                                          (((int) original_symbols[2] & 192) >> 6))];
        base64_symbols[3] = '=';
    } else if (valid_bytes == 1) {
        base64_symbols[2] = '=';
        base64_symbols[3] = '=';
    } else {
        base64_symbols[2] = base64_table[((((int) original_symbols[1] & 15) << 2) |
                                          (((int) original_symbols[2] & 192) >> 6))];
        base64_symbols[3] = base64_table[((int) original_symbols[2] & 63)];
    }
}

void finding_commands(int *i, int argc, char *argv[], int *e_cmd, int *d_cmd, int *i_cmd, int *f_cmd, int *f_cmd_num, int *check_fail) {
    for (*i = 1; *i < argc - 2; (*i)++) {
        if (argv[*i][0] == '-') {
            switch (argv[*i][1]) {
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
                    *f_cmd_num = atoi(argv[*i]);
                    if (argc == 5) {
                        print_help();
                        *check_fail = 1;
                    }
                    break;
                default:
                    print_help();
                    *check_fail = 1;
                    break;
            }
        } else {
            print_help();
            *check_fail = 1;
        }
    }
}

void decoding(long int file_size, int i_cmd, const unsigned char *original_file, char *base64_symbols, unsigned char *original_symbols, FILE *outfile) {
    int j = 0;
    int codes[256] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
                      52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27,
                      28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    while (j < file_size) {
        for (int p = 0; p < 4; p++) {
            if (i_cmd == 1) {
                int base64_symbol = 0;
                if (codes[(int) original_file[j + p]] != -1) {
                    base64_symbol = 1;
                }
                if (base64_symbol == 0) {
                    j++;
                    p--;
                } else
                    base64_symbols[p] = original_file[j + p];
            } else
                base64_symbols[p] = original_file[j + p];
        }
        int m = 0;
        int break_num = 0;
        for (int p = 0; p < 4; p++) {
            if (base64_symbols[p] == '=') {
                if (break_num == 0)
                    break_num = p;
            } else if (codes[(int) base64_symbols[p]] != -1)
                base64_symbols[p] = (char) codes[(int) base64_symbols[p]];
        }
        converting_to_original_symbols(break_num, original_symbols, base64_symbols, &m);
        if (j < file_size)
            for (int i = 0; i < m; i++) {
                fputc(original_symbols[i], outfile);
            }
        j += 4;
    }
}

void encoding(long int file_size, unsigned char *original_symbols, char *base64_symbols, const unsigned char *original_file, FILE *outfile, int f_cmd, int f_cmd_num) {
    int j = 0;
    int symbols_amount = 0;
    while (j < file_size) {
        int valid_bytes = 0;
        for (int p = 0; p < 3; p++) {
            original_symbols[p] = '\0';
        }
        for (int p = 0; p < 3; p++) {
            if (j + p < file_size) {
                original_symbols[p] = original_file[j + p];
                valid_bytes++;
            }
        }
        converting_to_base64_symbols(base64_symbols, original_symbols, valid_bytes);
        for (int p = 0; p < 4; p++) {
            fputc(base64_symbols[p], outfile);
            if (f_cmd == 1) {
                symbols_amount++;
                if ((symbols_amount == f_cmd_num) && (((p == 3) && (j + 3 < file_size)) || (p != 3))) {
                    fputc('\n', outfile);
                    symbols_amount = 0;
                }
            }
        }
        j += 3;
    }
}

int main(int argc, char *argv[]) {
    if ((argc > 3) && (argc < 7)) {
        int i;
        int e_cmd = 0, d_cmd = 0, i_cmd = 0, f_cmd = 0, f_cmd_num = -1;
        int check_fail = 0;
        finding_commands(&i, argc, argv, &e_cmd, &d_cmd, &i_cmd, &f_cmd, &f_cmd_num, &check_fail);
        if (check_fail == 0) {
            FILE *infile = fopen(argv[i], "rb");
            FILE *outfile = fopen(argv[i + 1], "wb");
            fseek(infile, 0, SEEK_END);
            long int file_size = ftell(infile);
            rewind(infile);
            unsigned char *original_file;
            original_file = (unsigned char *) malloc(sizeof(unsigned char) * file_size);
            fread(original_file, sizeof(unsigned char), (size_t) file_size, infile);
            unsigned char *original_symbols = (unsigned char *) malloc(3 * sizeof(char));
            char *base64_symbols = (char *) malloc(4 * sizeof(char));
            if (e_cmd == 1) {
                encoding(file_size, original_symbols, base64_symbols, original_file, outfile, f_cmd,
                         f_cmd_num);
            } else if (d_cmd == 1) {

                decoding(file_size, i_cmd, original_file, base64_symbols, original_symbols, outfile);
            }
        }
    }
    else print_help();
    return 0;
}