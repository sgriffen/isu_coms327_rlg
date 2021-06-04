DIR_BASE				= 
DIR_SRC					= $(DIR_BASE)src/
DIR_CONFIG 				= $(DIR_SRC)config/
DIR_UTILS 				= $(DIR_SRC)utils/
DIR_GAME				= $(DIR_SRC)game/
DIR_GAME_CLASSDEF 		= $(DIR_SRC)game/classdef/
DIR_GAME_UTILS 			= $(DIR_SRC)game/utils/
DIR_GAME_UTILS_CLASSDEF = $(DIR_SRC)game/utils/classdef/

DEPENDENCIES_H = 	$(DIR_BASE)main.h $\
					$(DIR_CONFIG)color.h $\
					$(DIR_CONFIG)config.h $\
					$(DIR_UTILS)math_utils.h $\
					$(DIR_UTILS)str_utils.h $\
					$(DIR_GAME_CLASSDEF)state.h $\
					$(DIR_GAME_CLASSDEF)dungeon.h $\
					$(DIR_GAME_CLASSDEF)room.h $\
					$(DIR_GAME_CLASSDEF)cell.h $\
					$(DIR_GAME_CLASSDEF)coordinate.h $\
					$(DIR_GAME_CLASSDEF)character.h $\
					$(DIR_GAME_CLASSDEF)item.h $\
					$(DIR_GAME_UTILS)character_movement.h $\
					$(DIR_GAME_UTILS)pathfinder.h $\
					$(DIR_GAME_UTILS_CLASSDEF)fibheap.h $\
					$(DIR_GAME_UTILS_CLASSDEF)queue.h $\
					$(DIR_GAME_UTILS_CLASSDEF)dice.h $\

DEPENDENCIES_C = 	$(DIR_BASE)main.cpp $\
					$(DIR_UTILS)math_utils.cpp $\
					$(DIR_UTILS)str_utils.cpp $\
					$(DIR_GAME)state.cpp $\
					$(DIR_GAME)dungeon.cpp $\
					$(DIR_GAME)room.cpp $\
					$(DIR_GAME)cell.cpp $\
					$(DIR_GAME)coordinate.cpp $\
					$(DIR_GAME)character.cpp $\
					$(DIR_GAME)item.cpp $\
					$(DIR_GAME_UTILS)dice.cpp $\
					$(DIR_GAME_UTILS)state_io_terminal.cpp $\
					$(DIR_GAME_UTILS)state_io_disk.cpp $\
					$(DIR_GAME_UTILS)state_draw.cpp $\
					$(DIR_GAME_UTILS)dungeon_io_disk.cpp $\
					$(DIR_GAME_UTILS)room_io_disk.cpp $\
					$(DIR_GAME_UTILS)cell_io_disk.cpp $\
					$(DIR_GAME_UTILS)character_movement.cpp $\
					$(DIR_GAME_UTILS)character_io_disk.cpp $\
					$(DIR_GAME_UTILS)item_io_disk.cpp $\
					$(DIR_GAME_UTILS)pathfinder.cpp $\
					$(DIR_GAME_UTILS)fibheap.cpp $\
					$(DIR_GAME_UTILS)queue.cpp $\

OBJS 	= rlg
CFLAGS 	= -Wall -Werror -g3
LIBS	= -lncurses
CC		= g++

all: $(OBJS)

rlg: $(DEPENDENCIES_H) $(DEPENDENCIES_C)
	$(CC) $(DEPENDENCIES_C) $(CFLAGS) $(LIBS) -o $@

#tests: tests/test_queue tests/test_fibheap
#
#tests/test_queue: tests/test_queue.cpp $(DIR_GAME_UTILS_CLASSDEF)queue.h $(DIR_GAME_UTILS)queue.cpp
#	$(CC) $(CFLAGS) tests/test_queue.cpp -o tests/test_queue
#
#tests/test_fibheap: tests/test_fibheap.cpp $(DIR_GAME_UTILS_CLASSDEF)fibheap.h $(DIR_GAME_UTILS)fibheap.cpp
#	$(CC) $(CFLAGS) tests/test_fibheap.cpp -o tests/test_fibheap

clean:
	-rm -f -r *.o $(OBJS)