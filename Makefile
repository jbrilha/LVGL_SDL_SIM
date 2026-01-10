.PHONY: clean SDL2 SDL3

all: SDL3 run

SDL2:
	@echo "Building with SDL2"
	@mkdir -p build
	cd build && cmake -DUSE_SDL2=ON .. && make

SDL3:
	@echo "Building with SDL3"
	@mkdir -p build
	cd build && cmake -DUSE_SDL2=OFF .. && make

clean:
	rm -rf build

run:
	./build/main
