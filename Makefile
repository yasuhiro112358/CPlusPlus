CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
SRCS = src/main.cpp src/calculator.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = build/myapp

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o build/myapp
