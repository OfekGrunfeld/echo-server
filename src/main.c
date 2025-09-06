#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "args.h"
#include "echo.h"

#define LISTEN_BACKLOG 1
#define CHUNK  65536

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(const int argc, const char *argv[]) {
    Args args;
    if (parse_args(argc, argv, &args))
        return EXIT_FAILURE;

    socklen_t client_address_size;
    struct sockaddr_in server_address = {0}, client_address = {0};

    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        handle_error("socket");

    const bool one = true;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(args.port);

    if (bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address)))
        handle_error("bind");

    if (listen(server_fd, LISTEN_BACKLOG))
        handle_error("listen");

    client_address_size = sizeof(client_address);
    const int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
    if (client_fd < 0)
        handle_error("accept");

    int rc = echo_splice(client_fd);
    close(client_fd); close(server_fd);

    return (rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
