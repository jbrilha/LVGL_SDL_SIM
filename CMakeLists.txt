cmake_minimum_required(VERSION 3.10)
project(lvgl_sdl_sim)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/widgets)
add_subdirectory(widgets)

add_subdirectory(lvgl)

add_executable(main main.cpp)

option(USE_SDL2 "Build with SDL2 instead of SDL3" OFF)

if(USE_SDL2)
    if(APPLE)
        # gotta use sdl2-config on macOS to avoid rpath issues
        execute_process(
            COMMAND sdl2-config --cflags
            OUTPUT_VARIABLE SDL2_CFLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND sdl2-config --libs
            OUTPUT_VARIABLE SDL2_LIBS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        # this one is just to be able to include SDl2/SDL.h without an extra ifguard
        execute_process(
            COMMAND sdl2-config --prefix
            OUTPUT_VARIABLE SDL2_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        string(REPLACE " " ";" SDL2_CFLAGS_LIST ${SDL2_CFLAGS})
        
        target_include_directories(main PRIVATE ${SDL2_PREFIX}/include)
        target_compile_options(main PRIVATE ${SDL2_CFLAGS_LIST})
        target_link_libraries(main PRIVATE lvgl all_widgets ${SDL2_LIBS})
    else()
        find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
        target_link_libraries(main PRIVATE lvgl all_widgets SDL2::SDL2)
    endif()
        
    target_compile_definitions(main PRIVATE USE_SDL2=1)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
    target_link_libraries(main PRIVATE lvgl all_widgets SDL3::SDL3)
endif()
