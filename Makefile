CXXFLAGS=-std=c++0x -Wall -g

main.exe: main.o 
	g++ $(CXXFLAGS) main.o -o main.exe

# Per il codice templato e' importante mettere i file .h
# tra le dipendenze per far rilevare a make le modifiche
# al codice della classe
main.o: main.cpp binarytree.h
	g++ $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clean

clean:
	rm main.exe *.o