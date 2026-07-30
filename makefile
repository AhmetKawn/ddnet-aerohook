CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2
LDFLAGS ?= -lglfw -lGL
all:
	$(CXX) $(CXXFLAGS) main.cpp -o app $(LDFLAGS)
