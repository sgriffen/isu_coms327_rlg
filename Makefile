all: rouge.o

rouge.o: rouge.c rouge.h src/dungeon.c src/dungeon.h src/room.c src/room.h src/cell.c src/cell.h src/coordinate.c src/coordinate.h src/character.c src/character.h src/utils/math_utils.c src/utils/math_utils.h src/utils/file_utils.c src/utils/file_utils.h  src/utils/pathfinder.c src/utils/pathfinder.h src/utils/queue.c src/utils/queue.h src/utils/movement.c src/utils/movement.h
	gcc -Wall -Werror -g3 rouge.c src/dungeon.c src/room.c src/cell.c src/coordinate.c src/character.c src/utils/math_utils.c src/utils/file_utils.c src/utils/pathfinder.c src/utils/queue.c src/utils/movement.c -o rouge.o

clean:
	rm -f *.o