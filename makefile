CXX		 := g++
BUILD    := ./build
TARGET   := raytracing.out
INCLUDE  := -I.

raytracing: ./src/main.cpp
	$(CXX) -o $(TARGET) ./src/main.cpp $(INCLUDE)
