CPP=gcc
CPPFLAGS=-O3 -ggdb3

all : main

main : main.o polygone.o
	$(CPP) $(CPPFLAGS) -o main main.o polygone.o -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lGLU -lGL -lX11  -lXt -lXi -lXmu -lXext -lX11 -lm 

main.o : main.cpp polygone.cpp
	$(CPP) $(CPPFLAGS) -o main.o -c main.cpp 
polygone.o : polygone.cpp
	$(CPP) $(CPPFLAGS) -o polygone.o -c polygone.cpp
clean :
	rm -f *.o
	rm -f main
