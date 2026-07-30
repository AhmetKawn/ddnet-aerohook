CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2
LDFLAGS ?= -lglfw -lGL

SRC = \
	main.cpp \
	core/map.cpp \
	render/render.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o app $(LDFLAGS)