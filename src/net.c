#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <echo/net.h>

int net_listen_tcp_any(const uint16_t port, const int backlog) {
    const int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd < 0) {
        perror("socket");
        return -1;
    }

    const int enable = 1;
    if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        handle_socket_error("setsockopt", s_fd);
        return -1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(s_fd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        handle_socket_error("bind", s_fd);
        return -1;
    }
    if (listen(s_fd, backlog) < 0) {
        handle_socket_error("listen", s_fd);
        return -1;
    }
    return s_fd;
}

int net_accept(const int listen_fd) {
    for (;;) {
        const int c = accept(listen_fd, NULL, NULL);
        if (c >= 0)
            return c;
        perror("accept");
        return -1;
    }
}