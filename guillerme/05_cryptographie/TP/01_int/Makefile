CC = gcc
LFLAGS = -Wall -lm -lncurses
CFLAGS = -c
DFLAGS = 
DEBUG = 0

ifeq (DEBUG, 1)
	DFLAGS += -g
endif

PGRM = crypt

$(PGRM): crypt.o menu.o main.o
	$(CC) -o $@ $(DFLAGS) $(LFLAGS) $^

%.o: %.h %.c
	$(CC) $(DFLAGS) $(LFLAGS) $(CFLAGS) $^

clean:
	rm -rf *.o *~ *.h.gch

sclean: clean
	rm $(PGRM)
