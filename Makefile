all: rouge.o

rouge.o: rouge.c rouge.h level.c level.h cell.c cell.h utils.c utils.h
	gcc -Wall -Werror -g3 rouge.c level.c cell.c utils.c -o rouge.o
	
test.o: test.c rouge.h level.c level.h cell.c cell.h utils.c utils.h
	gcc -Wall -Werror -g3 test.c level.c cell.c utils.c -o test.o
	
clean:
	rm -f *.o