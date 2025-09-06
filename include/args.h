#pragma once

#include <stdint.h>

typedef struct {
    uint16_t port;
} Args;

int parse_args(const int argc, const char *argv[], Args *out);
