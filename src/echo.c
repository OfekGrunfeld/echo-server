#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <echo/echo.h>

#ifndef CHUNK
#define CHUNK (1 << 20) // 1 MB
#endif

int echo_splice(const int client_fd) {
    int p[2] = {-1, -1};
    if (pipe(p) < 0) {
        perror("pipe");
        return -1;
    }

    for (;;) {
        const ssize_t n = splice(client_fd, NULL, p[1], NULL, CHUNK,
                                 SPLICE_F_MORE | SPLICE_F_MOVE);
        if (n == 0) // EOF
            break;
        ssize_t done = 0;
        while (done < n) {
            const ssize_t m = splice(p[0], NULL, client_fd, NULL, n - done,
                                     SPLICE_F_MORE | SPLICE_F_MOVE);
            done += m;
        }
    }
    close(p[0]);
    close(p[1]);
    return 0;
}
