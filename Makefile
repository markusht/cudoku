CFLAGS=-Wall -Wextra -pedantic

cudoku: cudoku.c
	$(CC) $(CFLAGS) -std=c11 -ggdb -o cudoku cudoku.c
