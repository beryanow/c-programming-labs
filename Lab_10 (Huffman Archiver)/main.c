#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

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
