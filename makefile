CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2
LDFLAGS ?= -lglfw -lGL

SRC = \
	main.cpp \
	core/map.cpp \
	render/render.cpp

APP = app

all: build

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(APP) $(LDFLAGS)

run: build
	./$(APP)

clean:
	rm -f $(APP)