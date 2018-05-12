#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

s_node *join_nodes(s_node *in_left, s_node *in_right) {
    s_node *joined_node = (s_node *) malloc(sizeof(s_node));

    joined_node->value = -1;
    joined_node->amount = in_left->amount + in_right->amount;
    joined_node->left = in_left;
    joined_node->right = in_right;

    return joined_node;
};

int comparator(const void *x1, const void *x2) {
    return (*(s_node **) x1)->amount - (*(s_node **) x2)->amount;
}

void print_sym_code(char *sym_code, FILE *outfile) {
    unsigned char buff = 0;
    int buff_size = 0;
    int k = 0;

    while (sym_code[k] != '\0') {
        if (buff_size == 8) {
            fwrite(&buff, 1, 1, outfile);
            buff_size = 0;
            buff = 0;
        }

        buff = (unsigned char) (((buff << 1) & 254) | ((sym_code[k] == '1') ? 1 : 0));
        buff_size++;
        k++;

        if ((sym_code[k] == '\0') && (buff_size > 0)) {
            buff = buff << (8 - buff_size);
            fwrite(&buff, 1, 1, outfile);
        }
    }
}

void print_encoded_sequence(int b, const char *code, FILE *outfile) {
    int j = 0;
    unsigned char buff = 0;
    int buff_size = 0;

    while (j < b) {
        if (buff_size == 8) {
            fwrite(&buff, 1, 1, outfile);
            buff_size = 0;
            buff = 0;
        }

        buff = (unsigned char) (((buff << 1) & 254) | ((code[j] == '1') ? 1 : 0));
        buff_size++;
        j++;

        if ((j == b) && (buff_size > 0)) {
            buff = buff << (8 - buff_size);
            fwrite(&buff, 1, 1, outfile);
        }
    }
}

s_node *make_node(int in_value, int in_amount) {
    s_node *new_node = (s_node *) malloc(sizeof(s_node));

    new_node->value = in_value;
    new_node->amount = in_amount;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
};

void make_codes(s_node *in_node, char *in_temp_code, int *t_c_index, char **arr_codes) {
    if ((in_node->left == NULL) && (in_node->right == NULL)) {
        arr_codes[in_node->value][0] = '0';
    } else {
        if (in_node->left->value == -1) {
            in_temp_code[*t_c_index] = '0';
            (*t_c_index)++;
            make_codes(in_node->left, in_temp_code, t_c_index, arr_codes);
        } else {
            in_temp_code[*t_c_index] = '0';
            int y = 0;
            while (in_temp_code[y] != '\0') {
                arr_codes[in_node->left->value][y] = in_temp_code[y];
                y++;
            }
        }
        if (in_node->right->value == -1) {
            in_temp_code[*t_c_index] = '1';
            (*t_c_index)++;
            make_codes(in_node->right, in_temp_code, t_c_index, arr_codes);
        } else {
            in_temp_code[*t_c_index] = '1';
            int y = 0;
            while (in_temp_code[y] != '\0') {
                arr_codes[in_node->right->value][y] = in_temp_code[y];
                y++;
            }

        }
        in_temp_code[*t_c_index] = '\0';
        (*t_c_index)--;
    }
}

void correct_bytes(int *h, int *d, int *code_length, int *f) {
    (*h)++;
    (*d)--;
    if (*h / 8 > 0) {
        *h = 0;
        *code_length -= 8;
        (*f)++;
        (*d) = 7;
    }
}

void restore_tree(s_node *in_node, unsigned char *sym_code, int *h, int *in_code_length, unsigned char in_sym, int *f, int *d, int *in_check) {
    *in_check = 0;
    while (*h < *in_code_length) {
        if ((sym_code[*f] & (1 << *d)) == 0) {
            if (in_node->left == NULL) {
                in_node->left = make_node(-2, 0);

                correct_bytes(h, d, in_code_length, f);
                restore_tree(in_node->left, sym_code, h, in_code_length, in_sym, f, d, in_check);
            } else {
                correct_bytes(h, d, in_code_length, f);
                restore_tree(in_node->left, sym_code, h, in_code_length, in_sym, f, d, in_check);
            }
        } else if ((sym_code[*f] & (1 << *d)) == (1 << *d)) {
            if (in_node->right == NULL) {
                in_node->right = make_node(-2, 0);

                correct_bytes(h, d, in_code_length, f);
                restore_tree(in_node->right, sym_code, h, in_code_length, in_sym, f, d, in_check);
            } else {
                correct_bytes(h, d, in_code_length, f);
                restore_tree(in_node->right, sym_code, h, in_code_length, in_sym, f, d, in_check);
            }
        }
        *in_check = 1;
    }
    if (*in_check == 0)
        in_node->value = in_sym;
}

void print_help() {
    printf("USAGE:\n [for decoding]: -d inputfilename outputfilename \n "
           "[for encoding]: -e inputfilename outputfrilename \n "
           "(!) inputfilename must not be empty");
}

void finding_commands(int argc, char *argv[], int *e_cmd, int *d_cmd, int *check_fail) {
    for (int i = 1; i < argc - 2; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'e':
                    *e_cmd = 1;
                    break;
                case 'd':
                    *d_cmd = 1;
                    break;
                default:
                    *check_fail = 1;
                    break;
            }
        } else {
            *check_fail = 1;
        }
    }
}

void add_code_parts(char **in_arr_codes, unsigned char *original_file, int i, char *code, int *k, FILE *outfile) {
    int t = 0;
    while (in_arr_codes[original_file[i]][t] != '\0') {
        code[*k] = in_arr_codes[original_file[i]][t];
        (*k)++;
        t++;
        if (*k == STEP) {
            print_encoded_sequence(*k, code, outfile);
            *k = 0;
            for (int l = STEP - 1; l >= 0; l--) {
                code[l] = '\0';
            }
        }
    }
}