CC=gcc
OFLAGS=-c -lgmp -g

pgrm: crypt.o menu.o main.o
	$(CC) -g -lgmp -o $@ $^

main.o: main.c
	$(CC) $(OFLAGS) $^

crypt.o: crypt.h crypt.c
	$(CC) $(OFLAGS) $^

menu.o: menu.h menu.c
	$(CC) $(OFLAGS) $^
