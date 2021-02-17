all: rouge.o

rouge.o: rouge.c rouge.h src/dungeon.c src/dungeon.h src/character.c src/character.h src/room.c src/room.h src/cell.c src/cell.h src/utils/math_utils.c src/utils/math_utils.h src/utils/file_utils.c src/utils/file_utils.h
	gcc -Wall -Werror -g3 rouge.c src/dungeon.c src/character.c src/room.c src/cell.c src/utils/math_utils.c src/utils/file_utils.c -o rouge.o
	
clean:
	rm -f *.o