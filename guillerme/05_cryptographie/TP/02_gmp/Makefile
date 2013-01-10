CC=gcc
LFLAGS= -lgmp -Wall
CFLAGS= -c
DFLAGS = 
DEBUG = 0

ifeq (DEBUG, 1)
	DFLAG += -g
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
