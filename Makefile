OPTIONS=-Wall -ansi -pedantic -std=gnu89
LIBS=-lX11 -lXtst -lusb-1.0
EXE=bbcursordriver
OUTPUT=-o $(EXE)
CC=gcc

main: clean
	$(CC) $(OUTPUT) $(OPTIONS) main.c $(LIBS)

clean:
	if [ -e $(EXE) ] ; then  rm $(EXE) ; fi
