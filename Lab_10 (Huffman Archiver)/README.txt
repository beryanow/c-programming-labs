DESCRIPTION:
Create a simple archiver that fulfils Huffman algorithm.  

USAGE:
-c infilename outfilename
to encode a random file infilename to the Huffman-encoded outfilename file.

-d infilename outfilename
to decode the Huffman-encoded infilename file to outfilename of the initial state. 

If the input string is not correct or the infilename is unreachable, then show the help message.

INPUT:
File with random data, containing symbols of [0..255] codes.

OUTPUT:
Encoded or decoded file. 

RESTRICTIONS:
Executable file size and data size are not more than 1024 Kb (each taken separately).

#include <stdio.h>
#include <stdlib.h>

#define STEP 1024

struct node {
	int value;
	int amount;
	struct node *left;
	struct node *right;
};

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
		}
		else {
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

		buff = (unsigned char)(((buff << 1) & 254) | ((sym_code[k] == '1') ? 1 : 0));
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

		buff = (unsigned char)(((buff << 1) & 254) | ((code[j] == '1') ? 1 : 0));
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
	}
	else {
		if (in_node->left->value == -1) {
			in_temp_code[*t_c_index] = '0';
			(*t_c_index)++;
			make_codes(in_node->left, in_temp_code, t_c_index, arr_codes);
		}
		else {
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
		}
		else {
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
			}
			else {
				correcting_bytes(h, d, code_length, f);
				restoring_tree(p->left, ar, h, code_length, r, f, d, m);
			}
		}
		else if ((ar[*f] & (1 << *d)) == (1 << *d)) {
			if (p->right == NULL) {
				p->right = make_node(-2, 0);
				correcting_bytes(h, d, code_length, f);
				restoring_tree(p->right, ar, h, code_length, r, f, d, m);
			}
			else {
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

		struct node *final_tree = make_node(-2, 0);
		for (int y = 0; y < coded_sym_amount; y++) {
			unsigned char sym;
			fread(&sym, sizeof(char), 1, infile);
			int code_length = 0;
			fread(&code_length, sizeof(char), 1, infile);
			unsigned char *code_sequence = (unsigned char *)calloc(sizeof(unsigned char), (size_t)code_length / 9 + 1);
			fread(code_sequence, sizeof(char), (size_t)code_length / 9 + 1, infile);

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
					if ((current_node->value) != -2) {
						fputc(current_node->value, outfile);
						current_node = start_node;
						p++;
						if (p == orig_f_size)
							break;
					}
				}
				else {
					current_node = current_node->right;
					if ((current_node->value) != -2) {
						fputc(current_node->value, outfile);
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

void encoding(char* argv[]) {
	int l = 2;

	FILE *infile = fopen(argv[l], "rb");
	FILE *outfile = fopen(argv[l + 1], "wb");

	fseek(infile, 0, SEEK_END);
	long temp_file_size = ftell(infile);
	long file_size = temp_file_size;
	rewind(infile);

	if (file_size != 0) {
		unsigned char *original_file;
		original_file = (unsigned char *)malloc(sizeof(unsigned char)* file_size);

		int arr[256];
		for (int i = 0; i < 256; i++) {
			arr[i] = 0;
		}

		while (temp_file_size > 0) {
			if ((temp_file_size / STEP) > 0) {
				fread(original_file, sizeof(unsigned char), STEP, infile);
				for (int i = 0; i < STEP; i++) {
					arr[original_file[i]]++;
				}
				temp_file_size -= STEP;
			}
			else {
				fread(original_file, sizeof(unsigned char), temp_file_size % STEP, infile);
				for (int i = 0; i < temp_file_size % STEP; i++) {
					arr[original_file[i]]++;
				}
				temp_file_size = 0;
			}
		}

		int x = 0;
		for (int i = 0; i < 256; i++) {
			if (arr[i] != 0) {
				x++;
			}
		}

		int sym_amount = x;
		struct node **arr_nodes = (struct node **) calloc(sizeof(struct node *), x);

		x = 0;
		for (int i = 0; i < 256; i++) {
			if (arr[i] > 0) {
				arr_nodes[x] = make_node(i, arr[i]);
				x++;
			}
		}

		while (x != 1) {
			qsort(arr_nodes, x, sizeof(struct node *), comparator);
			arr_nodes[0] = join_nodes(arr_nodes[0], arr_nodes[1]);
			for (int i = 1; i < x - 1; i++) {
				arr_nodes[i] = arr_nodes[i + 1];
			}
			arr_nodes[x - 1] = NULL;
			x--;
		}

		char *temp_code = (char *)calloc(sizeof(char), 8);
		int t_c_index = 0;

		char **arr_codes = (char **)malloc(sizeof(char *)* 256);
		for (int i = 0; i < 256; i++) {
			arr_codes[i] = (char *)calloc(sizeof(char), 8);
		}

		make_codes(arr_nodes[0], temp_code, &t_c_index, arr_codes);

		fwrite(&file_size, sizeof(int), 1, outfile);
		fwrite(&sym_amount, sizeof(int), 1, outfile);

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

		char *code = (char *)calloc(sizeof(char), STEP);

		rewind(infile);
		int k = 0;
		while (file_size > 0) {
			if ((file_size / STEP) > 0) {
				fread(original_file, sizeof(unsigned char), STEP, infile);
				for (int i = 0; i < STEP; i++) {
					int t = 0;
					while (arr_codes[original_file[i]][t] != '\0') {
						code[k] = arr_codes[original_file[i]][t];
						k++;
						t++;
						if (k == STEP) {
							print_encoded_sequence(k, code, outfile);
							k = 0;
							for (int l = STEP - 1; l >= 0; l--) {
								code[l] = '\0';
							}
						}
					}
				}
				file_size -= STEP;
			}
			else {
				fread(original_file, sizeof(unsigned char), file_size % STEP, infile);
				for (int i = 0; i < file_size % STEP; i++) {
					int t = 0;
					while (arr_codes[original_file[i]][t] != '\0') {
						code[k] = arr_codes[original_file[i]][t];
						k++;
						t++;
						if (k == STEP) {
							print_encoded_sequence(k, code, outfile);
							k = 0;
							for (int l = STEP - 1; l >= 0; l--) {
								code[l] = '\0';
							}
						}
					}
				}
				file_size = 0;
			}
		}
		if (k != 0) {
			print_encoded_sequence(k, code, outfile);
		}
	}
}

int main(int argc, char *argv[]) {

	int check_fail = 0;
	int e_cmd, d_cmd;
	finding_commands(argc, argv, &e_cmd, &d_cmd, &check_fail);

	if (check_fail == 0) {
		if (e_cmd == 1)
			encoding(argv);
		else
			decoding(argv);
	}
	else
		print_help();

	return 0;
}
