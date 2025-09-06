#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "args.h"
#include "echo.h"
#include "net.h"

#define LISTEN_BACKLOG 1
#define CHUNK  65536

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(const int argc, const char *argv[]) {
    Args args;
    if (parse_args(argc, argv, &args)) return EXIT_FAILURE;

    const int s_fd = net_listen_tcp_any(args.port, LISTEN_BACKLOG);
    if (s_fd < 0) return EXIT_FAILURE;

    const int c_fd = net_accept(s_fd);
    if (c_fd < 0) return EXIT_FAILURE;


    const int rc = echo_splice(c_fd);
    close(c_fd);
    close(s_fd);

    return (rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
