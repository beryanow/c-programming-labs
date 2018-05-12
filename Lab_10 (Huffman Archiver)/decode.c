#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void decode_data(char *argv[]) {
    int m = 2;

    FILE *infile = fopen(argv[m], "rb");
    FILE *outfile = fopen(argv[m + 1], "wb");

    int check_fail = 0;
    if (infile == NULL) {
        print_help();
        check_fail = 1;
    }

    if (check_fail == 0) {
        int orig_f_size = 0;
        int coded_sym_amount = 0;

        fread(&orig_f_size, sizeof(int), 1, infile);
        fread(&coded_sym_amount, sizeof(int), 1, infile);

        s_node *final_tree = make_node(-2, 0);
        for (int y = 0; y < coded_sym_amount; y++) {
            unsigned char sym;
            fread(&sym, sizeof(char), 1, infile);
            int code_length = 0;
            fread(&code_length, sizeof(char), 1, infile);

            int z = 0;
            if (code_length % 8 == 0)
                z = code_length / 8;
            else
                z = code_length / 8 + 1;

            unsigned char *code_sequence = (unsigned char *) calloc(sizeof(unsigned char), (size_t) z);
            fread(code_sequence, sizeof(char), (size_t) z, infile);

            int h = 0; // code_length index
            int f = 0; // coded symbol's bytes index
            int d = 7; // the required bit's index
            int check; // monitors whether the symbol has been already placed into the tree
            restore_tree(final_tree, code_sequence, &h, &code_length, sym, &f, &d, &check);
        }

        s_node *start_node = final_tree;
        s_node *current_node = start_node;

        int p = 0; // follows the number of original file's symbols
        while (p < orig_f_size) {
            char sym;
            fread(&sym, sizeof(char), 1, infile);
            for (int x = 7; x >= 0; x--) {
                if ((sym & (1 << x)) == 0) {
                    current_node = current_node->left;
                    if ((current_node->value) != -2) {
                        fputc(current_node->value, outfile);
                        current_node = start_node;
                        p++;
                        if (p == orig_f_size)
                            break;
                    }
                } else {
                    current_node = current_node->right;
                    if ((current_node->value) != -2) {
                        fputc(current_node->value, outfile);
                        current_node = start_node;
                        p++;
                        if (p == orig_f_size)
                            break;
                    }
                }
            }
        }
    }
}