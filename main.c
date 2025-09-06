#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>

#define LISTEN_BACKLOG 1
#define CHUNK  65536
#define PORT 12347

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(void) {
    socklen_t client_address_size;
    struct sockaddr_in server_address, client_address;

    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        handle_error("socket");

    const bool one = true;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &one , sizeof(one));

    struct sockaddr_in addr = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address)))
        handle_error("bind");

    if (listen(server_fd, LISTEN_BACKLOG))
        handle_error("listen");

    client_address_size = sizeof(client_address);
    const int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
    if (client_fd < 0)
        handle_error("accept");

    int p[2];
    if (pipe(p) < 0)
        handle_error("pipe");

    while (1) {
        const ssize_t n = splice(client_fd, NULL, p[1], NULL, CHUNK, SPLICE_F_MOVE | SPLICE_F_MORE);
        if (n == 0) break;

        ssize_t written = 0;
        while (written < n) {
            const ssize_t m = splice(p[0], NULL, client_fd, NULL, n - written, SPLICE_F_MOVE | SPLICE_F_MORE);
            written += m;
        }
    }
    close(p[0]);
    close(p[1]);
    close(client_fd);
    close(server_fd);
}
