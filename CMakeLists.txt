cmake_minimum_required(VERSION 3.18)
project(Mravenisko C)

set(CMAKE_C_STANDARD 99)

add_executable(Mravenisko main.c client/client.c client/client.h definition.c definition.h server/server.c server/server.h world/world.c world/world.h world/plocha/policko/policko.c world/plocha/policko/policko.h world/plocha/plocha.c world/plocha/plocha.h world/mravce/mravec/mravec.c world/mravce/mravec/mravec.h world/mravce/mravce.c world/mravce/mravce.h world/vlakna/worldPlayer/world_player.c world/vlakna/worldPlayer/world_player.h world/vlakna/worldThreadData.h world/vlakna/worldUser/world_user.c world/vlakna/worldUser/world_user.h)
target_link_libraries(Mravenisko pthread m)
