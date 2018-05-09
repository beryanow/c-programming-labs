#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

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

void printing_encoded_sequence(int b, const char *code, FILE *outfile) {
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

struct node *make_node(int in_value, int in_amount) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));

    new_node->value = in_value;
    new_node->amount = in_amount;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
};

struct node *join_nodes(struct node *in_left, struct node *in_right) {
    struct node *joined_node = (struct node *) malloc(sizeof(struct node));

    joined_node->value = -1;
    joined_node->amount = in_left->amount + in_right->amount;
    joined_node->left = in_left;
    joined_node->right = in_right;

    return joined_node;
};

int comparator(const void *x1, const void *x2) {
    return (*(struct node **) x1)->amount - (*(struct node **) x2)->amount;
}

void make_codes(struct node *in_node, char *in_temp_code, int *t_c_index, char **arr_codes) {
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

void correcting_bytes(int *h, int *d, int *code_length, int *f) {
    (*h)++;
    (*d)--;
    if (*h / 8 > 0) {
        *h = 0;
        *code_length -= 8;
        (*f)++;
        (*d) = 7;
    }
}

void restoring_tree(struct node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m) {
    *m = 0;
    while (*h < *code_length) {
        if ((ar[*f] & (1 << *d)) == 0) {
            if (p->left == NULL) {
                p->left = make_node(-2, 0);
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            } else {
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            }
        } else if ((ar[*f] & (1 << *d)) == (1 << *d)) {
            if (p->right == NULL) {
                p->right = make_node(-2, 0);
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->right, ar, h, code_length, r, f, d, m);
            } else {
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->right, ar, h, code_length, r, f, d, m);
            }
        }
        *m = 1;
    }
    if (*m == 0)
        p->value = r;
}

void print_help() {
    printf("USAGE:\n [for decoding]: -d inputfilename outputfilename \n "
           "[for encoding]: -e inputfilename outputfrilename \n "
           "(!) inputfilename must not be empty");
}