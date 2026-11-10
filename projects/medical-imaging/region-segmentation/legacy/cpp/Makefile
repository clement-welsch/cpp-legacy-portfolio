DIRPGM=.
CC=g++
CCFLAGS=-O2 -O3 -Wall -ggdb3

all : clean main

main :  OutilsPGM.o
	$(CC) $(CCFLAGS) -o main main.cpp $(DIRPGM)/OutilsPGM.o -I$(DIRPGM) -lm -lglut -lGLU -lGL

OutilsPGM.o : 
	$(CC) $(CCFLAGS) -o $(DIRPGM)/OutilsPGM.o -c $(DIRPGM)/OutilsPGM.c -I$(DIRPGM)  
	
clean :
	rm -f main
	rm -f $(DIRPGM)/*.o
