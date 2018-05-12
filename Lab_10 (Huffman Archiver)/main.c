#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char *argv[]) {

    int check_fail = 0;
    int e_cmd, d_cmd;
    finding_commands(argc, argv, &e_cmd, &d_cmd, &check_fail);

    if (check_fail == 0) {
        if (e_cmd == 1)
            encode_data(argv);
        else
            decode_data(argv);
    } else
        print_help();

    return 0;
}