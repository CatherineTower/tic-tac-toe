all: $(shell ls *.[ch]) Makefile
	clear
	gcc -Wall -Wextra -g -o tic-tac-toe main.c board.c interface.c -lncurses

test: main_test interface_test board_test

main_test: $(shell ls *.[ch]) Makefile
	gcc -Wall -Wextra -g -DUNITTEST_MAIN -o test main.c board.c interface.c -lncurses
	./test

interface_test: interface.c board.c interface.h Makefile
	gcc -Wall -Wextra -g -DUNITTEST_INTERFACE -o test interface.c board.c -lncurses
	./test

board_test: board.c board.h Makefile
	gcc -Wall -Wextra -g -DUNITTEST_BOARD -o test board.c
	./test

.PHONY: clean
clean:
	rm -f tic-tac-toe test *.o
