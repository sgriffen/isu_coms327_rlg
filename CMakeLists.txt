cmake_minimum_required(VERSION 3.17)
project(hw_01 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(src)
include_directories(src/utils)

add_executable(hw_01
        src/utils/file_utils.c
        src/utils/file_utils.h
        src/utils/math_utils.c
        src/utils/math_utils.h
        src/utils/movement.c
        src/utils/movement.h
        src/utils/pathfinder.c
        src/utils/pathfinder.h
        src/utils/queue.c
        src/utils/queue.h
        src/cell.c
        src/cell.h
        src/character.c
        src/character.h
        src/coordinate.c
        src/coordinate.h
        src/dungeon.c
        src/dungeon.h
        src/room.c
        src/room.h
        rouge.c
        rouge.h)
