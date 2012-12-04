CC = gcc
LFLAGS = -Wall -lm -lncurses
CFLAGS = -c
DFLAGS = 
DEBUG = 0

ifeq (DEBUG, 1)
	DFLAGS += -g
endif

crypt: crypt.o menu.o main.o
	$(CC) -o $@ $(DFLAGS) $(LFLAGS) $^

%.o: %.c
	$(CC) $(DFLAGS) $(LFLAGS) $(CFLAGS) $^

clean:
	rm -rf *.o *~

