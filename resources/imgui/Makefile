#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL
#

CXX = g++
#CXX = clang++

EXE = example_sdl_opengl3
SOURCES = main.cc
SOURCES += imgui_impl_sdl.cc imgui_impl_opengl3.cc
SOURCES += imgui.cc imgui_demo.cc imgui_draw.cc imgui_widgets.cc
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -Wall
LIBS =

##---------------------------------------------------------------------
## OPENGL LOADER
##---------------------------------------------------------------------

## Using OpenGL loader: glew
## (This assumes a system-wide installation)
CXXFLAGS += -lGLEW -DIMGUI_IMPL_OPENGL_LOADER_GLEW

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	# LIBS += -lGL -ldl `sdl2-config --libs`
	LIBS += -lGL `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../../%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:../libs/gl3w/GL/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:../libs/glad/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE) clean
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)
