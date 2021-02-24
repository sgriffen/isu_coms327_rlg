all: rouge.o

rouge.o: rouge.c rouge.h src/dungeon.c src/dungeon.h src/character.c src/character.h src/room.c src/room.h src/cell.c src/cell.h src/utils/math_utils.c src/utils/math_utils.h src/utils/file_utils.c src/utils/file_utils.h src/utils/queue.c src/utils/queue.h src/utils/pathfinder.c src/utils/pathfinder.h
	gcc -Wall -Werror -g3 rouge.c src/dungeon.c src/character.c src/room.c src/cell.c src/utils/math_utils.c src/utils/file_utils.c src/utils/queue.c src/utils/pathfinder.c -o rouge.o

tests: queue_test.o
	
queue_test.o: tests/queue_test.c src/utils/queue.c src/utils/queue.h src/cell.c src/cell.h src/utils/math_utils.c src/utils/math_utils.h
	gcc -Wall -Werror -g3 tests/queue_test.c src/utils/queue.c src/cell.c src/utils/math_utils.c -o tests/queue_test.o

clean:
	rm -f *.o
	
clean-tests:
	rm -f tests/*.o