#ifndef NEW_FUNCTIONS_H
#define NEW_FUNCTIONS_H

struct node {
    int value;
    int amount;
    struct node *left;
    struct node *right;
};

struct node *make_node(int in_value, int in_amount);
struct node *join_nodes(struct node *in_left, struct node *in_right);

int comparator(const void *x1, const void *x2);

void make_codes(struct node *in_node, char *in_temp_code, int *t_c_index, char **arr_codes);
void correcting_bytes(int *h, int *d, int *code_length, int *f);
void restoring_tree(struct node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m);
void finding_commands(int argc, char *argv[], int *e_cmd, int *d_cmd, int *check_fail);
void print_sym_code(char *sym_code, FILE *outfile);
void printing_encoded_sequence(int b, const char *code, FILE *outfile);
void print_help();
void decoding(char *argv[]);
void encoding (char* argv[]);

#endif
