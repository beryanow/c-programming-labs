#ifndef PROJECT_4_FUNCTIONS_H
#define PROJECT_4_FUNCTIONS_H

typedef struct node {
    int key;
    int times;
    struct node *left;
    struct node *right;
} s_node;

struct sequence {
    char cod[256];
    int otm;
};

void encoding(char *argv[]);

void decoding(char *argv[]);

void print_help();

s_node *new_tree_node(int key, int amount);

void encoding_symbols(s_node *p, char *seq, int *j, struct sequence *arr3, int *max);

void finding_commands(int argc, char *argv[], int *c_cmd, int *d_cmd, int *check_fail);

void correcting_bytes(int *h, int *d, int *code_length, int *f);

void restoring_tree(s_node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m);

s_node *connecting_nodes(int h, s_node *initial_nodes);

void printing_sym_code(struct sequence *sym_codes, const unsigned char *original_file, int j, FILE *outfile);

void printing_encoded_sequence(int b, const char *code, FILE *outfile);

void qsort_implementation(int *arr, int size, s_node *m);


#endif
