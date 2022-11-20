SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.hpp)
OBJS := ${SRCS:.cpp=.o}

LDFLAGS := -lGL -lGLU -lglfw
CFLAGS := -Wall
CXX := g++

TARGET := main

.PHONY := clean

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -rf *.o
	rm $(TARGET)