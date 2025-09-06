cmake_minimum_required(VERSION 4.0)
project(echo_server C)

set(CMAKE_C_STANDARD 17)

add_executable(echo_server
        src/args.c
        src/main.c
        src/echo.c
        src/net.c
)
