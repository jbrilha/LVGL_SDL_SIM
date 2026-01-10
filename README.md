# LVGL SDL Simulator

Minimal implementation to allow easy testing of LVGL widgets, using SDL as the rendering engine.
**Supports SDL2 and SDL3!**

## Requirements

- SDL2 or SDL3, naturally
- CMake 3.10+

## Usage

First clone the repo and make sure you get the LVGL submodule:

```bash
git clone --recurse-submodules https://github.com/jbrilha/LVGL_SDL_SIM.git
```

Then use CMake as you would normally:

```bash
mkdir -p build
cd build && cmake .. && make
```

The default option is to use SDL3, but if you want to use SDL2:

```bash
mkdir -p build
cd build && cmake -DUSE_SDL2=ON .. && make
```

Or just use the Makefile:

```bash
make SDL3 run
# or
make SDL2 run
```


The executable will be placed in `build/`, so to run it:

```bash
./build/main
```

Finally, there are some example widgets in the `widgets/` directory, so to add your own simply follow the pattern of adding the subdirectory and linking the library in `widgets/CMakeLists.txt`.
You then call them at the end of `init_lvgl()`, or after that function returns.

## Supported LVGL versions

Frankly I didn't check for any other than the current master branch, which at the time of writing is version 9.4.
Either way the widget logic is completely encapsulated in `widgets/` so you have free rein to (up/down)grade the LVGL version accordingly.

## Why?

I tried (to no avail) to use the [existing](https://docs.lvgl.io/9.4/details/integration/pc/index.html) options for running on PC, but it was more frustrating than it should be for me to just get it running.
Also did not like being tied to an IDE to do this, go figure.
