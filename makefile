# VORALDO_FLAGS = $(shell pkg-config sdl2 --cflags --libs) -O3 -std=c++11 -lGLEW -lGL -lSDL2 -lSDL2_ttf
VORALDO_FLAGS = $(shell pkg-config sdl2 --cflags --libs) -O3 -std=c++11 -lGLEW -lGL -lSDL2

all: msg main clean

msg:
		@echo
		@echo 'Compiling starts on '
		@date
		@echo

main: lodepng.o perlin.o utils.o voraldo1_0.o
		g++ -o main resources/code/main.cc *.o               ${VORALDO_FLAGS}

voraldo1_0.o: resources/code/voraldo1_0.h resources/code/voraldo1_0.cc
		g++ -c -o voraldo1_0.o resources/code/voraldo1_0.cc  ${VORALDO_FLAGS}

utils.o: resources/code/voraldo1_0.h resources/code/voraldo1_0_utils.cc
		g++ -c -o utils.o resources/code/voraldo1_0_utils.cc ${VORALDO_FLAGS}

debug.o: resources/code/voraldo1_0.h resources/code/debug.cc
		g++ -c -o debug.o resources/code/debug.cc            ${VORALDO_FLAGS}

perlin.o: resources/code/voraldo1_0.h resources/code/perlin.cc
		g++ -c -o perlin.o resources/code/perlin.cc          ${VORALDO_FLAGS}

lodepng.o: resources/code/lodepng.h resources/code/lodepng.cc
		g++ -c -o lodepng.o resources/code/lodepng.cc        ${VORALDO_FLAGS}

run: msg
		time ./main

clean:
		@echo
		@echo 'Compiling ends on '
		@date
		@echo
		@rm *.o
		@echo 'executable size:'
		@stat --printf "%s bytes\n" main
		@echo
