#ifndef PROJECT_2_FUNCTIONS_H
#define PROJECT_2_FUNCTIONS_H

#define STEP 2048 // must be divisible by 8

typedef struct node {
    int value;
    int amount;
    struct node *left;
    struct node *right;
} s_node;

s_node *join_nodes(s_node *in_left, s_node *in_right);

s_node *make_node(int in_value, int in_amount);

int comparator(const void *x1, const void *x2);

void make_codes(s_node *in_node, char *in_temp_code, int *t_c_index, char **arr_codes, int *single_check);

void print_sym_code(char *sym_code, FILE *outfile);

void print_encoded_sequence(int b, const char *code, FILE *outfile);

void correct_bytes(int *h, int *d, int *code_length, int *f);

void restore_tree(s_node *in_node, unsigned char *sym_code, int *h, int *in_code_length, unsigned char in_sym, int *f, int *d, int *in_check);

void print_help();

void finding_commands(int argc, char *argv[], int *e_cmd, int *d_cmd, int *check_fail);

void add_code_parts(char **in_arr_codes, unsigned char *original_file, int i, char *code, int *k, FILE *outfile);

void encode_data(char *argv[]);

void decode_data(char *argv[]);

#endif
