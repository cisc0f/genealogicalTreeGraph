CC=g++
FLAGS=-Wall
VERSION=-std=c++14
OUTPUT=gen

genRun: main.o genogram.o
	$(CC) $(VERSION) $(FLAGS) main.o genogram.o -o $(OUTPUT)
	./gen

genBuild: main.o genogram.o
	$(CC) $(VERSION) $(FLAGS) main.o genogram.o -o $(OUTPUT)

main.o: main.cpp
	$(CC) $(VERSION) -c main.cpp

genogram.o: genogram.cpp genogram.h
	$(CC) $(VERSION) -c genogram.cpp

clean:
	rm *.o $(OUTPUT)