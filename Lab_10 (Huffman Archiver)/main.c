#include <stdio.h>
#include <stdlib.h>

void print_help() {
    printf("USAGE:\n [for decoding]: -d inputfilename outputfilename \n "
                   "[for encoding]: -c inputfilename outputfrilename \n "
                   "(!) inputfilename must not be empty");
}

struct node {
    int key;
    int times;
    struct node *left;
    struct node *right;
};

struct sequence {
    char cod[256];
    int otm;
};

struct nodes_pointers {
    struct node *p;
};

struct node *new_tree_node(int key, int amount) {
    struct node *new_1 = (struct node *) malloc(sizeof(struct node));

    new_1->key = key;
    new_1->times = amount;
    new_1->left = NULL;
    new_1->right = NULL;

    return new_1;
}

struct node *stick_tree_node(struct node *l, struct node *r, int amount) {
    struct node *new_1 = (struct node *) malloc(sizeof(struct node));

    new_1->times = amount;
    new_1->left = l;
    new_1->right = r;

    return new_1;
}

void encoding_symbols(struct node *p, char *seq, int *j, struct sequence *arr3, int *max) {
    if (p->left != NULL) {
        seq[*j] = '0';
        (*j)++;
        encoding_symbols(p->left, seq, j, arr3, max);
    }

    if (p->right != NULL) {
        seq[*j] = '1';
        (*j)++;
        encoding_symbols(p->right, seq, j, arr3, max);
    } else {
        if (*j > *max) {
            *max = *j;
        }
        for (int v = 0; v < *j; v++) {
            arr3[p->key].cod[v] = seq[v];
            arr3[p->key].otm = 0;
        }
    }
    seq[*j] = '\0';
    (*j)--;
}

void finding_commands(int argc, char *argv[], int *c_cmd, int *d_cmd, int *check_fail) {
    for (int i = 1; i < argc - 2; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'c':
                    *c_cmd = 1;
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
                p->left = new_tree_node(-2, 0);
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            } else {
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            }
        } else if ((ar[*f] & (1 << *d)) == (1 << *d)) {
            if (p->right == NULL) {
                p->right = new_tree_node(-2, 0);
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
        p->key = r;
}

struct nodes_pointers *connecting_nodes(int h, struct node *initial_nodes) {
    struct nodes_pointers *set_tree = (struct nodes_pointers *) malloc(sizeof(struct nodes_pointers) * h);

    for (int i = 0; i < h; i++) {
        set_tree[i].p = new_tree_node(initial_nodes[i].key, initial_nodes[i].times);
    }

    while (h != 1) {
        set_tree[1].p = stick_tree_node(set_tree[0].p, set_tree[1].p, set_tree[0].p->times + set_tree[1].p->times);
        for (int i = 1; i < h; i++)
            set_tree[i - 1] = set_tree[i];
        h--;
        for (int i = 0; i < h - 1; i++) {
            for (int k = i + 1; k < h; k++) {
                if (set_tree[i].p->times > set_tree[k].p->times) {
                    struct node *temp = set_tree[i].p;
                    set_tree[i].p = set_tree[k].p;
                    set_tree[k].p = temp;
                }
            }
        }
    }

    return set_tree;
}

void printing_sym_code(struct sequence *sym_codes, const unsigned char *original_file, int j, FILE *outfile) {
    unsigned char buff = 0;
    int buff_size = 0;
    int k = 0;
    while (sym_codes[original_file[j]].cod[k] != '\0') {
        if (buff_size == 8) {
            fwrite(&buff, 1, 1, outfile);
            buff_size = 0;
            buff = 0;
        }

        buff = (unsigned char) (((buff << 1) & 254) | ((sym_codes[original_file[j]].cod[k] == '1') ? 1 : 0));
        buff_size++;
        k++;

        if ((sym_codes[original_file[j]].cod[k] == '\0') && (buff_size > 0)) {
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

void encoding(char *argv[]) {
    int arr[256];
    for (int i = 0; i < 256; i++)
        arr[i] = 0;

    int i = 2;
    int check_fail = 0;

    FILE *infile = fopen(argv[i], "rb");
    FILE *outfile = fopen(argv[i + 1], "wb");
    if (infile == NULL) {
        print_help();
        check_fail = 1;
    }

    if (check_fail == 0) {
        fseek(infile, 0, SEEK_END);
        long file_size = ftell(infile);
        rewind(infile);

        unsigned char *original_file;
        original_file = (unsigned char *) malloc(sizeof(unsigned char) * file_size);
        fread(original_file, sizeof(unsigned char), (size_t) file_size, infile);

        int j = 0;
        int h = 0;
        while (j < file_size) {
            if (arr[original_file[j]] == 0)
                h++;
            arr[original_file[j]]++;
            j++;
        }

        int y = h; // amount of coded symbols of each kind

        struct node *initial_nodes = (struct node *) malloc(sizeof(struct node) * h);

        j = 0;
        for (i = 0; i < 256; i++) {
            if (arr[i] > 0) {
                initial_nodes[j].key = i;
                initial_nodes[j].times = arr[i];
                j++;
            }
        }

        for (i = 0; i < h - 1; i++) {
            for (int k = i + 1; k < h; k++) {
                if (initial_nodes[i].times > initial_nodes[k].times) {
                    struct node temp = initial_nodes[i];
                    initial_nodes[i] = initial_nodes[k];
                    initial_nodes[k] = temp;
                }
            }
        }

        struct nodes_pointers *set_tree = connecting_nodes(h, initial_nodes);

        char *temp_seq = (char *) malloc(sizeof(char) * 8);

        struct sequence *sym_codes = (struct sequence *) calloc(sizeof(struct sequence), 256);

        int max;
        j = 0;

        encoding_symbols(set_tree[0].p, temp_seq, &j, sym_codes, &max);

        if (y == 1) {
            sym_codes[set_tree[0].p->key].cod[0] = '1';
            sym_codes[set_tree[0].p->key].otm = 0;
        }
        char *code = (char *) calloc(sizeof(char), (size_t) max * file_size);

        fwrite(&file_size, sizeof(int), 1, outfile);
        fwrite(&y, sizeof(int), 1, outfile);

        int b = 0; // used for following the encoded sequence's length
        j = 0;

        while (j < file_size) {
            int k = 0; // used for following the symbol code's length

            if ((sym_codes[original_file[j]].cod[k] != '\0') && (sym_codes[original_file[j]].otm != 1)) {

                int t = 0; // symbol code's length
                while (sym_codes[original_file[j]].cod[k] != '\0') {
                    t++;
                    k++;
                }

                fwrite(&original_file[j], sizeof(char), 1, outfile);
                fwrite(&t, sizeof(char), 1, outfile);

                sym_codes[original_file[j]].otm = 1;

                printing_sym_code(sym_codes, original_file, j, outfile);
            }

            k = 0;
            while (sym_codes[original_file[j]].cod[k] != '\0') {
                code[b] = sym_codes[original_file[j]].cod[k];
                k++;
                b++;
            }

            j++;
        }

        printing_encoded_sequence(b, code, outfile);
    }
}

void decoding(char *argv[]) {

    int i = 2;
    FILE *infile = fopen(argv[i], "rb");
    FILE *outfile = fopen(argv[i + 1], "wb");

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

        struct node *final_tree = new_tree_node(-2, 0);

        for (int y = 0; y < coded_sym_amount; y++) {
            unsigned char sym;
            fread(&sym, sizeof(char), 1, infile);

            int code_length = 0;
            fread(&code_length, sizeof(char), 1, infile);

            unsigned char *code_sequence = (unsigned char *) calloc(sizeof(unsigned char),
                                                                    (size_t) code_length / 9 + 1);
            fread(code_sequence, sizeof(char), (size_t) code_length / 9 + 1, infile);

            int h = 0; // used for following code_length
            int f = 0; // used for following needed amount of the coded symbol's bytes
            int d = 7; // used for observing the bit required
            int check; // used for monitoring whether the symbol has been already placed into the tree

            restoring_tree(final_tree, code_sequence, &h, &code_length, sym, &f, &d, &check);
        }

        struct node *start_node = final_tree;
        struct node *current_node = start_node;

        int p = 0; // used for following the number of original file's symbols

        while (p < orig_f_size) {
            char sym;
            fread(&sym, sizeof(char), 1, infile);

            for (int x = 7; x >= 0; x--) {
                if ((sym & (1 << x)) == 0) {
                    current_node = current_node->left;
                    if ((current_node->key) != -2) {
                        fputc(current_node->key, outfile);
                        current_node = start_node;
                        p++;
                        if (p == orig_f_size)
                            break;
                    }
                } else {
                    current_node = current_node->right;
                    if ((current_node->key) != -2) {
                        fputc(current_node->key, outfile);
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

int main(int argc, char *argv[]) {

    int check_fail = 0;
    int c_cmd, d_cmd;

    finding_commands(argc, argv, &c_cmd, &d_cmd, &check_fail);

    if (check_fail == 0) {
        if (c_cmd == 1)
            encoding(argv);
        else
            decoding(argv);
    } else
        print_help();

    return 0;
}

