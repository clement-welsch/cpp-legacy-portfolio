#Makefile Linux
EXEC = Domino.exe
COMPILO = g++

GLUT_INC= -I./Glut
GLUT_LIBS= -lopengl32 -L. -lglut32 -lglut
GL_INC=
#GL_LIBS= -lGL

#Chemin d'acces des includes
CXXINCS =  $(GLUT_INC) $(GL_INC)

#Parametres de compilation
CXXFLAGS = $(CXXINCS) -Wall

#Chemin d'acces des librairies
LIBS = $(GL_LIBS) $(GLUT_LIBS)

#Chemin d'acces des .h et .cpp (qui pourraient etre ailleurs que dans le répertoire principal)
PATH_INC=.
PATH_SRC=.

#Cree l'executable sous Linux
all: $(EXEC)

$(EXEC): main.o Domino.o texture.o tga.o ChuteDomino.o
	$(COMPILO) -o $@ $^ $(LIBS)
        
main.o : $(PATH_SRC)/main.cpp $(PATH_INC)/ChuteDomino.h texture.h
	$(COMPILO) -o $@ -c $< $(CXXFLAGS) $(CXXINCS)
        
Domino.o: Domino.cpp $(PATH_INC)/Domino.h 
	$(COMPILO) -o $@ -c $< $(CXXFLAGS) $(CXXINCS)

texture.o: texture.cpp $(PATH_INC)/ texture.h 
	$(COMPILO) -o $@ -c $< $(CXXFLAGS) $(CXXINCS)
        
tga.o: tga.cpp $(PATH_INC)/tga.h 
	$(COMPILO) -o $@ -c $< $(CXXFLAGS) $(CXXINCS)
        
ChuteDomino.o: ChuteDomino.cpp $(PATH_INC)/ChuteDomino.h 
	$(COMPILO) -o $@ -c $< $(CXXFLAGS) $(CXXINCS)

#Efface les fichiers .o
clean:
	rm -rf *.o
