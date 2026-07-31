CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2
PKG_CONFIG ?= pkg-config

SRC = \
	main.cpp \
	core/player.cpp \
	core/camera.cpp \
	engine/engine.cpp \
	render/render.cpp

APP = app

GLFW_CFLAGS := $(shell $(PKG_CONFIG) --cflags glfw3 2>/dev/null)
GLFW_LIBS := $(shell $(PKG_CONFIG) --libs glfw3 2>/dev/null)

ifeq ($(strip $(GLFW_LIBS)),)
GLFW_LIBS := -lglfw
endif

GLFW_LIBS += -lGL

CPPFLAGS += $(GLFW_CFLAGS)
LDLIBS += $(GLFW_LIBS)

all: build

build:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC) -o $(APP) $(LDFLAGS) $(LDLIBS)

run: build
	./$(APP)

clean:
	rm -f $(APP)

.PHONY: all build run clean