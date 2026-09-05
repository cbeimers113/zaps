#include "main.h"

#include <stdlib.h>
#include <string.h>

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow(
        "Zaps",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * CELL, HEIGHT * CELL, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Create hardware-accelerated streaming texture and its pixel buffer
    SDL_Texture *streamingTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888, // 32-bit format: 4 bytes per pixel
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH * CELL,
        HEIGHT * CELL);
    uint32_t *pixelBuffer = (uint32_t *)malloc(WIDTH * CELL * HEIGHT * CELL * sizeof(uint32_t));

    // Initialize the state
    sim_init();

    // Setup some parameters
    int mouse_x = 0, mouse_y = 0;
    const float SIM_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    Uint64 last = SDL_GetPerformanceCounter();

    // Run the main loop
    int running = 1;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = 0;
        }

        // Detect input events
        input_update();
        Uint32 mouse_button = SDL_GetMouseState(&mouse_x, &mouse_y);
        Input input = input_get(mouse_x, mouse_y, mouse_button);

        // Track simulation time
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last) / (float)SDL_GetPerformanceFrequency();
        last = now;

        // Update at a fixed 60Hz rate
        accumulator += dt;
        while (accumulator >= SIM_DT)
        {
            sim_update(input);
            accumulator -= SIM_DT;
        }

        // Render the updated space
        memset(pixelBuffer, 0, WIDTH * CELL * HEIGHT * CELL * sizeof(uint32_t));
        sim_render(renderer, pixelBuffer);

        // Copy pixel buffer to GPU texture memory and present the render
        SDL_UpdateTexture(streamingTexture, NULL, pixelBuffer, WIDTH * CELL * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    free(pixelBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// Render the simulation
void sim_render(SDL_Renderer *renderer, uint32_t *buffer)
{
    uint8_t *cells = sim_get()->cells;

    // Draw background
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    // Render cell grid
    SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);
    for (int gy = 0; gy < HEIGHT; gy++)
    {
        for (int gx = 0; gx < WIDTH; gx++)
        {
            if (cells && cells[gy * WIDTH + gx])
            {
                SDL_Rect rect = {.x = gx * CELL, .y = gy * CELL, .w = CELL, .h = CELL};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    int padding = 5;

    // Draw glyph selection menu on left
    for (int i = 0; i < 26; i++)
    {
        int x16 = padding + i * (GLYPH_SIZE_16 + padding);
        int y16 = padding;

        int x32 = padding + i * (GLYPH_SIZE_32 + padding);
        int y32 = padding * 2 + GLYPH_SIZE_16;

        glyph_render(x16, y16, 'A' + i, GLYPH_SIZE_16, buffer);
        glyph_render(x32, y32, 'A' + i, GLYPH_SIZE_32, buffer);
    }
}

// Draw a pixel to the buffer
void set_pixel(uint32_t *buffer, int x, int y, uint32_t color)
{
    BOUNDS_CHECK(x, y);
    buffer[x + y * WIDTH * CELL] = color;
}

// Draw a pixel to the buffer with decomposed RGBA parameters
void set_pixel_rgba(uint32_t *buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    BOUNDS_CHECK(x, y);
    uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;
    buffer[x + y * WIDTH * CELL] = color;
}

// Draw a rectangle to the buffer
void draw_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color)
{
    for (int yOffs = 0; yOffs < h; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        // Draw left and right edges
        set_pixel(buffer, x, yy, color);
        set_pixel(buffer, x + w, yy, color);

        // Draw top and bottom edges
        if (yOffs > 0 && yOffs < h - 1)
            continue;

        for (int xOffs = 0; xOffs < w; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            set_pixel(buffer, xx, yy, color);
        }
    }
}