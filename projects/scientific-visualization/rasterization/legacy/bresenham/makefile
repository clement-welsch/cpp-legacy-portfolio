CPP=gcc
CPPFLAGS=-O3 -ggdb3

all : main

main : main.o
	$(CPP) $(CPPFLAGS) -o main main.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXext -lX11 -lm 

main.o : main.cpp
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp
clean :
	rm -f *.o
	rm -f main
