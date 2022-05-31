include ./env

build: ./objects/main.o ./objects/CPU.o ./objects/MemOps.o ./objects/ALU.o
	g++ -o ./bin/main ./objects/* $(COMP_FLAGS)


./objects/main.o: ./headers/CPU.hpp ./headers/constants.hpp ./source/main.cpp
	g++ -o ./objects/main.o -c -I./headers/ ./source/main.cpp $(COMP_FLAGS)

./objects/CPU.o: ./headers/CPU.hpp ./headers/constants.hpp ./headers/Memory.hpp ./source/CPU.cpp ./source/MemOps.cpp ./source/ALU.cpp
	g++ -o ./objects/CPU.o -c -I./headers/ ./source/CPU.cpp $(COMP_FLAGS)

./objects/MemOps.o: ./headers/CPU.hpp ./headers/constants.hpp ./headers/Memory.hpp ./source/MemOps.cpp
	g++ -o ./objects/MemOps.o -c -I./headers/ ./source/MemOps.cpp $(COMP_FLAGS)

./objects/ALU.o: ./headers/CPU.hpp ./headers/constants.hpp ./headers/Memory.hpp ./source/ALU.cpp
	g++ -o ./objects/ALU.o -c -I./headers/ ./source/ALU.cpp $(COMP_FLAGS)

run:
	./bin/main

clean:
	- rm ./objects/*
	- rm ./bin/*
