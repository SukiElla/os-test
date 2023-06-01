CC=gcc
CFLAGS=-std=c89

main.o: main.c filesys.h 
	$(CC) $(CFLAGS) -c $< -o $@
