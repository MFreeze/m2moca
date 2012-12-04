CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall 



# analyse_vigenere :
analyse_vigenere: outil_vigenere.o analyse_vigenere.o
	$(CC) -o analyse_vigenere outil_vigenere.o analyse_vigenere.o $(LDFLAGS) 


analyse_vigenere.o: analyse_vigenere.c outil_vigenere.h
	$(CC) -c analyse_vigenere.c $(CFLAGS)



outil_vigenere.o: outil_vigenere.c outil_vigenere.h
	$(CC) -c outil_vigenere.c $(CFLAGS)

clean: 
	rm -rf *.o

mrproper: clean
	rm -rf analyse_vigenere 

