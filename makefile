CC = g++
FLAGS = -Wall -I./include
CPP = ./src/*.cpp
HEADERS = ./include/*.h

build: $(CPP) $(HEADERS)
	$(CC) $(FLAGS) -o compression.out $(CPP)

debug: $(CPP) $(HEADERS)
	$(CC) $(FLAGS) -g -o compression.out $(CPP)