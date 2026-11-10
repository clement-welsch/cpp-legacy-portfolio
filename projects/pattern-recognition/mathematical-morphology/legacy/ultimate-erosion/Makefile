DIRPGM=.
CC=gcc
CCFLAGS=-O2 -O3 -Wall

all : clean main

main :  OutilsPGM.o
	$(CC) $(CCFLAGS) -o main main.c $(DIRPGM)/OutilsPGM.o -I$(DIRPGM) -lm -lglut -lGLU -lGL

OutilsPGM.o : 
	$(CC) $(CCFLAGS) -o $(DIRPGM)/OutilsPGM.o -c $(DIRPGM)/OutilsPGM.c -I$(DIRPGM)  
	
clean :
	rm -f main
	rm -f $(DIRPGM)/*.o
