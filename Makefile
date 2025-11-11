CXX = g++
CC = gcc

CXXFLAGS = -std=c++2b -O3 -Wall -Wextra
CFLAGS = -O3 -Wall

TARGET = main
OBJS = main.o linenoise.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

linenoise.o: linenoise.c linenoise.h
	$(CC) $(CFLAGS) -c linenoise.c

run: all
	@./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
