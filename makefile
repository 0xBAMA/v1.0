VORALDO_FLAGS =  -Wall -O3 -std=c++17 -lGLEW -lGL -lstdc++fs $(shell pkg-config sdl2 --cflags --libs) -Wno-deprecated
IMGUI_FLAGS   =  -Wall -lGLEW -DIMGUI_IMPL_OPENGL_LOADER_GLEW `sdl2-config --cflags`

all: msg main clean run

msg:
		@echo
		@echo 'Compiling starts on '
		@date
		@echo

main: imgui lodepng.o perlin.o utils.o gpu_data.o voraldo1_0.o bigint
		g++ -o main resources/code/main.cc *.o resources/BigInt/*.o     ${VORALDO_FLAGS} 

imgui: resources/imgui/*
		g++ -c -o imgui_impl_sdl.o resources/imgui/imgui_impl_sdl.cc         ${IMGUI_FLAGS}
		g++ -c -o imgui_impl_opengl3.o resources/imgui/imgui_impl_opengl3.cc ${IMGUI_FLAGS}
		g++ -c -o imgui.o resources/imgui/imgui.cc                           ${IMGUI_FLAGS}
		g++ -c -o imgui_demo.o resources/imgui/imgui_demo.cc                 ${IMGUI_FLAGS}
		g++ -c -o imgui_draw.o resources/imgui/imgui_draw.cc                 ${IMGUI_FLAGS}
		g++ -c -o imgui_widgets.o resources/imgui/imgui_widgets.cc           ${IMGUI_FLAGS}
		@echo


voraldo1_0.o: resources/code/voraldo1_0.h resources/code/voraldo1_0.cc
		g++ -c -o voraldo1_0.o resources/code/voraldo1_0.cc              ${VORALDO_FLAGS}

gpu_data.o: resources/code/gpu_data.h resources/code/gpu_data.cc
		g++ -c -o gpu_data.o resources/code/gpu_data.cc                  ${VORALDO_FLAGS}

utils.o: resources/code/voraldo1_0.h resources/code/voraldo1_0_utils.cc
		g++ -c -o utils.o resources/code/voraldo1_0_utils.cc             ${VORALDO_FLAGS}

debug.o: resources/code/voraldo1_0.h resources/code/debug.cc
		g++ -c -o debug.o resources/code/debug.cc                        ${VORALDO_FLAGS}

perlin.o: resources/code/voraldo1_0.h resources/code/perlin.cc
		g++ -c -o perlin.o resources/code/perlin.cc                      ${VORALDO_FLAGS}

lodepng.o: resources/code/lodepng.h resources/code/lodepng.cc
		g++ -c -o lodepng.o resources/code/lodepng.cc                    ${VORALDO_FLAGS}

bigint:
		g++ -c -o resources/BigInt/BigUnsigned.o -O2 -Wno-deprecated resources/BigInt/BigUnsigned.cc
		g++ -c -o resources/BigInt/BigInteger.o -O2 -Wno-deprecated resources/BigInt/BigInteger.cc
		g++ -c -o resources/BigInt/BigIntegerAlgorithms.o -O2 -Wno-deprecated resources/BigInt/BigIntegerAlgorithms.cc
		g++ -c -o resources/BigInt/BigUnsignedInABase.o -O2 -Wno-deprecated resources/BigInt/BigUnsignedInABase.cc
		g++ -c -o resources/BigInt/BigIntegerUtils.o -O2 -Wno-deprecated resources/BigInt/BigIntegerUtils.cc



clean:
		@echo
		@echo 'Compiling ends on '
		@date
		@echo
		@rm *.o
		@echo 'executable size:'
		@stat --printf "%s bytes\n" main
		@echo

run:
		@./main
