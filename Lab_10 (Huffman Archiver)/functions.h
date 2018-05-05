#ifndef HUFFMAN_FUNCTIONS_H

typedef struct node {
    int key;
    int times;
    struct node *left;
    struct node *right;
} s_node;

struct sequence {
    unsigned char cod[256];
    int otm;
};

int comparator(const void *x1, const void *x2);

s_node *stick_tree_node(s_node *l, s_node *r, int amount);
s_node **connecting_nodes(int h, s_node *initial_nodes);
s_node *new_tree_node(int key, int amount);

void decoding(char *argv[]);
void print_help();
void printing_encoded_sequence(int b, const char *code, FILE *outfile);
void finding_commands(int argc, char *argv[], int *e_cmd, int *d_cmd, int *check_fail);
void encoding_symbols(s_node *p, char *seq, int *j, struct sequence *arr3, int *max);
void printing_sym_code(struct sequence *sym_codes, const unsigned char *original_file, int j, FILE *outfile);
void restoring_tree(s_node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m);
void correcting_bytes(int *h, int *d, int *code_length, int *f);
void encoding(char *argv[]);


#define HUFFMAN_FUNCTIONS_H

#endif
