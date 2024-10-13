CC=gcc
CFLAGS="-Wall"

debug:clean
	$(CC) $(CFLAGS) -g -o brickbreaker main.c
stable:clean
	$(CC) $(CFLAGS) -o brickbreaker main.c
clean:
	rm -vfr *~ brickbreaker
