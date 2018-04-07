#ifndef PROJECT_1_FUNCTIONS_H
#define PROJECT_1_FUNCTIONS_H

struct node {
    int key;
    int times;
    struct node *left;
    struct node *right;
};

struct nodes_pointers {
    struct node *p;
};

struct sequence {
    char cod[256];
    int otm;
};

void printing_sym_code(struct sequence *sym_codes, const unsigned char *original_file, int j, FILE *outfile);

void printing_encoded_sequence(int b, const char *code, FILE *outfile);

void decoding(char *argv[]);

void print_help();

struct node *new_tree_node(int key, int amount);

void restoring_tree(struct node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m);

void encoding(char *argv[]);

struct nodes_pointers *connecting_nodes(int h, struct node *initial_nodes);

void encoding_symbols(struct node *p, char *seq, int *j, struct sequence *arr3, int *max);

void finding_commands(int argc, char *argv[], int *c_cmd, int *d_cmd, int *check_fail);

void correcting_bytes(int *h, int *d, int *code_length, int *f);

#endif
