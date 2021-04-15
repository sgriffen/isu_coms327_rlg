DIR_BASE				= 
DIR_SRC					= $(DIR_BASE)src/
DIR_RES 				= $(DIR_SRC)res/
DIR_UTILS 				= $(DIR_SRC)utils/
DIR_GAME				= $(DIR_SRC)game/
DIR_GAME_CLASSDEF 		= $(DIR_SRC)game/classdef/
DIR_GAME_UTILS 			= $(DIR_SRC)game/utils/
DIR_GAME_UTILS_CLASSDEF = $(DIR_SRC)game/utils/classdef/

DEPENDENCIES_H = 	$(DIR_BASE)main.h $\
					$(DIR_GAME_UTILS_CLASSDEF)queue.h $\
					$(DIR_GAME_UTILS_CLASSDEF)dice.h $\
					$(DIR_GAME_UTILS)movement.h $\
					$(DIR_GAME_UTILS)pathfinder.h $\
					$(DIR_GAME_CLASSDEF)cell.h $\
					$(DIR_GAME_CLASSDEF)character.h $\
					$(DIR_GAME_CLASSDEF)item.h $\
					$(DIR_GAME_CLASSDEF)coordinate.h $\
					$(DIR_GAME_CLASSDEF)dungeon.h $\
					$(DIR_GAME_CLASSDEF)room.h $\
					$(DIR_GAME_CLASSDEF)state.h $\
					$(DIR_GAME_CLASSDEF)npc_template.h $\
					$(DIR_GAME_CLASSDEF)item_template.h $\
					$(DIR_RES)color.h $\
					$(DIR_RES)config.h $\
					$(DIR_UTILS)math_utils.h $\
					$(DIR_UTILS)str_utils.h $\

DEPENDENCIES_C = 	$(DIR_BASE)main.cpp $\
					$(DIR_GAME_UTILS)queue.cpp $\
					$(DIR_GAME_UTILS)dice.cpp $\
					$(DIR_GAME_UTILS)parse_io.cpp $\
					$(DIR_GAME_UTILS)movement.cpp $\
					$(DIR_GAME_UTILS)pathfinder.cpp $\
					$(DIR_GAME)cell.cpp $\
					$(DIR_GAME)character.cpp $\
					$(DIR_GAME)item.cpp $\
					$(DIR_GAME)coordinate.cpp $\
					$(DIR_GAME)dungeon.cpp $\
					$(DIR_GAME)room.cpp $\
					$(DIR_GAME)state.cpp $\
					$(DIR_GAME)npc_template.cpp $\
					$(DIR_GAME)item_template.cpp $\
					$(DIR_UTILS)math_utils.cpp $\
					$(DIR_UTILS)str_utils.cpp $\

OBJS 	= rlg
CFLAGS 	= -Wall -Werror -g3
LIBS	= -lncurses
CC		= g++

all: $(OBJS)

rlg: $(DEPENDENCIES_H) $(DEPENDENCIES_C)
	$(CC) $(CFLAGS) $(DEPENDENCIES_C) $(LIBS) -o $@

clean:
	-rm -f *.o $(OBJS)