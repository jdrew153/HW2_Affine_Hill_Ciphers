CC=g++
CXXFLAGS = -std=c++20 -Wall -g

TARGET = main

SRCS = $(TARGET).cpp $(wildcard src/*cpp)  $(wildcard utils/*.cpp)
HEADERS = $(wildcard src/*.h) $(wildcard utils/*.h)


all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	$(RM) $(TARGET)

run:
	./$(TARGET)
