cmake_minimum_required(VERSION 3.18)
project(Mravenisko C)

set(CMAKE_C_STANDARD 99)

add_executable(Mravenisko main.c client.c client.h definition.c definition.h server.c server.h)
target_link_libraries(Mravenisko pthread m)
