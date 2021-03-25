all: rouge.o

rouge.o: rouge.cpp rouge.h src/state.cpp src/state.h src/dungeon.cpp src/dungeon.h src/room.cpp src/room.h src/cell.cpp src/cell.h src/coordinate.cpp src/coordinate.h src/character.cpp src/character.h src/utils/math_utils.cpp src/utils/math_utils.h  src/utils/pathfinder.cpp src/utils/pathfinder.h src/utils/queue.cpp src/utils/queue.h src/utils/movement.cpp src/utils/movement.h src/utils/config.h
	g++ -Wall -Werror -g3 rouge.cpp src/state.cpp src/dungeon.cpp src/room.cpp src/cell.cpp src/coordinate.cpp src/character.cpp src/utils/math_utils.cpp src/utils/pathfinder.cpp src/utils/queue.cpp src/utils/movement.cpp -lncurses -o rouge.o

clean:
	rm -f *.o