#include "lvgl/lvgl.h"
#include "round_scale_widget.h"
#include <stdlib.h>
#include <unistd.h>

#if USE_SDL2
#include <SDL2/SDL.h>
#define QUIT_EVENT SDL_QUIT
#else
#include <SDL3/SDL.h>
#define QUIT_EVENT SDL_EVENT_QUIT
#endif

#include "cubic_bezier_widget.h"
#include "temp_widget.h"

#define WIDTH 240
#define HEIGHT 320

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static void sdl_display_flush(lv_display_t *disp, const lv_area_t *area,
                              uint8_t *px_map) {
    lv_draw_sw_rgb565_swap(px_map, (area->x2 - area->x1 + 1) *
                                       (area->y2 - area->y1 + 1));

    SDL_Rect rect;
    rect.x = area->x1;
    rect.y = area->y1;
    rect.w = area->x2 - area->x1 + 1;
    rect.h = area->y2 - area->y1 + 1;

    SDL_UpdateTexture(texture, &rect, px_map, rect.w * sizeof(uint16_t));
    SDL_RenderClear(renderer);
#if USE_SDL2
    SDL_RenderCopy(renderer, texture, NULL, NULL);
#else
    SDL_RenderTexture(renderer, texture, NULL, NULL);
#endif
    SDL_RenderPresent(renderer);

    lv_display_flush_ready(disp);
}

static void sdl_mouse_read(lv_indev_t *indev, lv_indev_data_t *data) {
#if USE_SDL2
    int mouse_x, mouse_y;
#else
    float mouse_x, mouse_y;
#endif

    uint32_t mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    data->point.x = mouse_x;
    data->point.y = mouse_y;

#if USE_SDL2
    data->state = (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
                      ? LV_INDEV_STATE_PRESSED
                      : LV_INDEV_STATE_RELEASED;
#else
    data->state = (mouse_state & SDL_BUTTON_LMASK) ? LV_INDEV_STATE_PRESSED
                                                   : LV_INDEV_STATE_RELEASED;
#endif
}

void init_sdl(void) {
    SDL_Init(SDL_INIT_VIDEO);

#if USE_SDL2
    window = SDL_CreateWindow("LVGL SDL2", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
#else
    window = SDL_CreateWindow("LVGL SDL3", WIDTH, HEIGHT, 0);
#endif

#if USE_SDL2
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#else
    renderer = SDL_CreateRenderer(window, NULL);
#endif
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
}

void run_sdl_loop(void) {
    bool running = true;
    uint32_t last_tick = SDL_GetTicks();
    uint32_t current_tick = 0, elapsed = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == QUIT_EVENT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
#if USE_SDL2
                if (event.key.keysym.sym == SDLK_q)
#else
                if (event.key.key == SDLK_Q)
#endif
                {
                    running = false;
                }
            }
        }

        current_tick = SDL_GetTicks();
        elapsed = current_tick - last_tick;

        lv_tick_inc(elapsed);
        last_tick = current_tick;

        lv_task_handler();
        SDL_Delay(5);
    }
}

void destroy_sdl(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void init_lvgl(void) {
    lv_init();

    lv_display_t *disp = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_flush_cb(disp, sdl_display_flush);

    size_t buf_size = WIDTH * HEIGHT * sizeof(uint16_t);
    void *buf1 = malloc(buf_size);
    lv_display_set_buffers(disp, buf1, NULL, buf_size,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, sdl_mouse_read);

    // ********** YOUR WIDGETS HERE ********** //
    // temp_widget_init(disp);
    cubic_bezier_widget_init(disp);
    // round_scale_widget_init(disp);
}

int main(int argc, char *argv[]) {
    init_sdl();
    init_lvgl();

    run_sdl_loop();

    destroy_sdl();

    return 0;
}
