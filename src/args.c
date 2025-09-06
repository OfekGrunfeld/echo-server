#include <stdio.h>
#include <stdlib.h>

#include "../include/args.h"

int parse_args(const int argc, const char *argv[], Args *out) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return -1;
    }
    char *end = NULL;
    out->port = (uint16_t)strtol(argv[1], &end, 10);;
    return 0;
}
