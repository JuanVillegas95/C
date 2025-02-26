CC = gcc
CFLAGS = -Wall -g

array: array.c
	$(CC) $(CFLAGS) -o test array.c
	./test

clean:
	rm -f test
