all: main

clean: 
	rm main
	rm *.o

main : main.cpp
	g++ -o  main  main.cpp -L/usr/X11R6/lib/ -lglut -lGLU -lGL -lX11 -lXext -lm
