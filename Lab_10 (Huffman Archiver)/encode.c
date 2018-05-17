#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void encode_data(char *argv[]) {
    int l = 2;

    FILE *infile = fopen(argv[l], "rb");
    FILE *outfile = fopen(argv[l + 1], "wb");

    fseek(infile, 0, SEEK_END);
    long temp_file_size = ftell(infile);
    long file_size = temp_file_size;
    rewind(infile);

    if (file_size != 0) {
        unsigned char *original_file;
        original_file = (unsigned char *) malloc(sizeof(unsigned char) * file_size);

        int arr[256];
        for (int i = 0; i < 256; i++) {
            arr[i] = 0;
        }

        while (temp_file_size > 0) {
            int t = fread(original_file, sizeof(unsigned char), STEP, infile);
            for (int i = 0; i < t; i++) {
                arr[original_file[i]]++;
            }
            temp_file_size -= t;
        }

        int x = 0;
        for (int i = 0; i < 256; i++) {
            if (arr[i] != 0) {
                x++;
            }
        }

        int sym_amount = x;
        s_node **arr_nodes = (s_node **) calloc(sizeof(s_node *), x);

        x = 0;
        for (int i = 0; i < 256; i++) {
            if (arr[i] > 0) {
                arr_nodes[x] = make_node(i, arr[i]);
                x++;
            }
        }

        while (x != 1) {
            qsort(arr_nodes, x, sizeof(s_node *), comparator);
            arr_nodes[x - 2] = join_nodes(arr_nodes[x - 1], arr_nodes[x - 2]);
            x--;
        }

        char *temp_code = (char *) calloc(sizeof(char), 256);
        int t_c_index = 0;

        char **arr_codes = (char **) malloc(sizeof(char *) * 256);
        for (int i = 0; i < 256; i++) {
            arr_codes[i] = (char *) calloc(sizeof(char), 256);
        }

        int single_check = 0;
        make_codes(arr_nodes[0], temp_code, &t_c_index, arr_codes, &single_check);

        fwrite(&file_size, sizeof(int), 1, outfile);
        fwrite(&sym_amount, sizeof(int), 1, outfile);

        if (single_check) {
            fwrite(&arr_nodes[0]->value, sizeof(char), 1, outfile);
        } else {
            for (int i = 0; i < 256; i++) {
                if (arr_codes[i][0] != '\0') {
                    int length = 0;
                    while (arr_codes[i][length] != '\0') {
                        length++;
                    }
                    fwrite(&i, sizeof(char), 1, outfile);
                    fwrite(&length, sizeof(char), 1, outfile);
                    print_sym_code(arr_codes[i], outfile);
                }
            }

            char *code = (char *) calloc(sizeof(char), STEP);

            rewind(infile);
            int k = 0;
            while (file_size > 0) {
                int t = fread(original_file, sizeof(unsigned char), STEP, infile);
                for (int i = 0; i < t; i++) {
                    add_code_parts(arr_codes, original_file, i, code, &k, outfile);
                }
                file_size -= t;
            }

            if (k != 0) {
                print_encoded_sequence(k, code, outfile);
            }
        }
    }
}