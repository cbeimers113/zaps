#include "main.h"

#include <stdlib.h>
#include <string.h>

int main()
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

    // Initialize the simulation and gui
    gui_init(sim_init());

    // Setup some parameters
    int mouseX = 0, mouseY = 0;
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
        Uint32 mouseB = SDL_GetMouseState(&mouseX, &mouseY);
        Input input = input_get(mouseX, mouseY, mouseB);

        // Track simulation time
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last) / (float)SDL_GetPerformanceFrequency();
        last = now;

        // Update at a fixed 60Hz rate
        accumulator += dt;
        while (accumulator >= SIM_DT)
        {
            sim_update(input);
            gui_update(input);
            accumulator -= SIM_DT;
        }

        // Render the updated space
        memset(pixelBuffer, 0, WIDTH * CELL * HEIGHT * CELL * sizeof(uint32_t));
        sim_render(pixelBuffer);
        gui_render(pixelBuffer);

        // Copy pixel buffer to GPU texture memory and present the render
        SDL_UpdateTexture(streamingTexture, NULL, pixelBuffer, WIDTH * CELL * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    gui_free();
    free(pixelBuffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
