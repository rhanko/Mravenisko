cmake_minimum_required(VERSION 3.18)
project(Mravenisko C)

set(CMAKE_C_STANDARD 99)

add_executable(Mravenisko main.c client/client.c client/client.h definition.c definition.h server/server.c server/server.h world/world.c world/world.h)
target_link_libraries(Mravenisko pthread m)
