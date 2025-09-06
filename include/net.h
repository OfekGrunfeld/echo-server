#pragma once

#include <stdint.h>

#define handle_socket_error(msg, fd) \
    do { perror(msg); close(fd); } while(0)

// returns listen fd or -1
int net_listen_tcp_any(uint16_t port, int backlog);

// returns client fd or -1
int net_accept(const int listen_fd);
