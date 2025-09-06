cmake_minimum_required(VERSION 4.0)
project(echo_server C)

add_library(core
        src/net.c
        src/echo.c
        src/args.c
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(echo_server src/main.c)

target_compile_definitions(echo_server PRIVATE _GNU_SOURCE)
target_compile_features(echo_server PRIVATE c_std_17)
target_link_libraries(echo_server PRIVATE core)