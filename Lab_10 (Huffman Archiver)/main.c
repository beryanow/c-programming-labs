#include <stdio.h>
#include <stdlib.h>

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

void going_through(struct node *p, char *seq, int *j, struct sequence *arr3, int* max) {
    if (p->left != NULL) {
        seq[*j] = '0';
        (*j)++;
        going_through(p->left, seq, j, arr3, max);
    }
    if (p->right != NULL) {
        seq[*j] = '1';
        (*j)++;
        going_through(p->right, seq, j, arr3, max);
    }
    else {
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

void finding_commands(int *i, int argc, char *argv[], int *c_cmd, int *d_cmd, int *check_fail) {
    for (*i = 1; *i < argc - 2; (*i)++) {
        if (argv[*i][0] == '-') {
            switch (argv[*i][1]) {
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
        }
        else {
            *check_fail = 1;
        }
    }
}


void going_t (struct node* p, unsigned char* ar, int* h, int* gg, char r, int* f, int* d, int* m) {
    *m = 0;
    while (*h < *gg) {
        if ((ar[*f] & (1 << *d)) == 0) {
            if (p->left == NULL) {
                p->left = new_tree_node(-1, 0);
                (*h)++;
                (*d)--;
                if (*h / 8 > 0) {
                    *h = 0;
                    *gg -= 8;
                    (*f)++;
                    (*d) = 7;
                }
                going_t(p->left, ar, h, gg, r, f, d, m);
            }
            else {
                (*h)++;
                (*d)--;
                if (*h / 8 > 0) {
                    *h = 0;
                    *gg -= 8;
                    (*f)++;
                    (*d) = 7;
                }
                going_t(p->left, ar, h, gg, r, f, d, m);
            }
        }
        else if ((ar[*f] & (1 << *d)) == (1 << *d)) {
            if (p->right == NULL) {
                p->right = new_tree_node(-1, 0);
                (*h)++;
                (*d)--;
                if (*h / 8 > 0) {
                    *h = 0;
                    *gg -= 8;
                    (*f)++;
                    (*d) = 7;
                }
                going_t(p->right, ar, h, gg, r, f, d, m);
            }
            else {
                (*h)++;
                (*d)--;
                if (*h / 8 > 0) {
                    *h = 0;
                    *gg -= 8;
                    (*f)++;
                    (*d) = 7;
                }
                going_t(p->right, ar, h, gg, r, f, d, m);
            }
        }
        *m = 1;
    }
    if (*m == 0)
        p->key = r;
}

int main(int argc, char *argv[]) {
    int arr[256];
    for (int i = 0; i < 256; i++)
        arr[i] = 0;
    int i, check_fail = 0;
    int c_cmd, d_cmd;
    finding_commands(&i, argc, argv, &c_cmd, &d_cmd, &check_fail);
    if (c_cmd == 1) {

        FILE *infile = fopen(argv[i], "rb");
        FILE *outfile = fopen(argv[i + 1], "wb");
        if (infile == NULL) {

            return 0;
        }
        fseek(infile, 0, SEEK_END);
        int file_size = ftell(infile);
        rewind(infile);
        unsigned char *original_file;
        original_file = (unsigned char *)malloc(sizeof(unsigned char)* file_size);
        fread(original_file, sizeof(unsigned char), (size_t)file_size, infile);

        int j = 0, h = 0;
        while (j < file_size) {
            if (arr[original_file[j]] == 0)
                h++;
            arr[original_file[j]]++;
            j++;
        }

        int y = h;
        struct node *arr2 = (struct node *) malloc(sizeof(struct node) * h);


        j = 0;
        for (i = 0; i < 256; i++) {
            if (arr[i] > 0) {
                arr2[j].key = i;
                arr2[j].times = arr[i];
                j++;
            }
        }

        for (i = 0; i < h - 1; i++) {
            for (int k = i + 1; k < h; k++) {
                if (arr2[i].times > arr2[k].times) {
                    struct node temp = arr2[i];
                    arr2[i] = arr2[k];
                    arr2[k] = temp;
                }
            }
        }

        struct nodes_pointers *trees = (struct nodes_pointers *) malloc(sizeof(struct nodes_pointers) * h);
        for (i = 0; i < h; i++) {
            trees[i].p = new_tree_node(arr2[i].key, arr2[i].times);
        }
        while (h != 1) {
            trees[1].p = stick_tree_node(trees[0].p, trees[1].p, trees[0].p->times + trees[1].p->times);
            for (i = 1; i < h; i++)
                trees[i - 1] = trees[i];
            h--;
            for (i = 0; i < h - 1; i++) {
                for (int k = i + 1; k < h; k++) {
                    if (trees[i].p->times > trees[k].p->times) {
                        struct node *temp = trees[i].p;
                        trees[i].p = trees[k].p;
                        trees[k].p = temp;
                    }
                }
            }
        }


        char *temp_seq = (char *)malloc(sizeof(char)* 8);

        struct sequence *seq = (struct sequence *) calloc(sizeof(struct sequence), 256);

        j = 0;
        int max;
        going_through(trees[0].p, temp_seq, &j, seq, &max);

        char *code = (char *)calloc(sizeof(char), max * file_size);

        j = 0;
        int b = 0;
        fwrite(&file_size, sizeof(int), 1, outfile);
        fwrite(&y, sizeof(char), 1, outfile);
        while (j < file_size) {
            int k = 0;
            if ((seq[original_file[j]].cod[k] != '\0') && (seq[original_file[j]].otm != 1)) {

                int t = 0;
                while (seq[original_file[j]].cod[k] != '\0') {
                    t++;
                    k++;
                }

                fwrite(&original_file[j], sizeof(char), 1, outfile);

                fwrite(&t, sizeof(char), 1, outfile);

                seq[original_file[j]].otm = 1;
                unsigned char buff = 0;
                int buff_size = 0;
                k = 0;
                while (seq[original_file[j]].cod[k] != '\0') {
                    t++;
                    if (buff_size == 8) {
                        fwrite(&buff, 1, 1, outfile);
                        buff_size = 0;
                    }
                    buff = (unsigned char) (((buff << 1) & 254) | ((seq[original_file[j]].cod[k] == '1') ? 1 : 0));
                    buff_size++;
                    k++;
                    if ((seq[original_file[j]].cod[k] == '\0') && (buff_size > 0)) {
                        buff = buff << (8 - buff_size);
                        fwrite(&buff, 1, 1, outfile);
                    }
                }

            }
            k = 0;
            while (seq[original_file[j]].cod[k] != '\0') {
                code[b] = seq[original_file[j]].cod[k];
                k++;
                b++;
            }
            j++;
        }



        j = 0;
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
    else {

        FILE *infile = fopen(argv[i], "rb");
        FILE *outfile = fopen(argv[i + 1], "wb");
        if (infile == NULL) {

            return 0;
        }
        fseek(infile, 0, SEEK_END);
        int file_size = ftell(infile);


        char* arr = (char*)calloc(sizeof(char), 33);
        int h = 0;
        unsigned char* f = (unsigned char*)malloc(sizeof(char) * file_size);
        rewind(infile);

        h = 0;

        int r1 = 0;
        int r2 = 0;
        fread(&r1, sizeof(int), 1, infile);
        fread(&r2, sizeof(char), 1, infile);
        struct node *arr2 = (struct node *) malloc(sizeof(struct node));
        arr2 = new_tree_node(-1, 0);
        for (int y = 0; y < r2; y++) {
            char ch;
            fread(&ch, sizeof(char), 1, infile);
            int gg = 0;
            fread(&gg, sizeof(char), 1, infile);
            unsigned char* arr1 = (unsigned char*)calloc(sizeof(unsigned char), gg / 9 + 1);
            fread(arr1, sizeof(char), gg / 9 + 1, infile);

            h = 0;
            int f = 0;
            int d = 7;
            int m;
            going_t(arr2, arr1, &h, &gg, ch, &f, &d, &m);
        }
        struct node* start = arr2;
        unsigned char* arr3 = (unsigned char*)malloc(sizeof(unsigned char) * file_size);
        int p = 0;
        struct node* current = start;
        while (p < r1) {
            char ch;
            fread(&ch, sizeof(char), 1, infile);
            for (int x = 7; x >= 0; x--) {
                if ((ch & (1 << x)) == 0) {
                    current = current->left;
                    if ((current->key) != -1) {
                        fputc(current->key, outfile);
                        current = start;
                        p++;
                        if (p == r1)
                            break;
                    }
                }
                else {
                    current = current->right;
                    if ((current->key) != -1) {
                        fputc(current->key, outfile);
                        current = start;
                        p++;
                        if (p == r1)
                            break;
                    }
                }
            }
        }
    }
    return 0;
}

