#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char** argv)
{
    // --- Initialize only video + events ---
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // --- Create a resizable, high-DPI-aware window ---
    SDL_WindowFlags flags =
        (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    SDL_Window* window = SDL_CreateWindow("Pure SDL3 Window", 800, 600, flags);
    if (!window) 
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // --- Create a renderer (let SDL pick the best backend) ---
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --- Main loop ---
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT ||
                e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                running = false;
            } else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                int w = 0, h = 0;
                SDL_GetWindowSize(window, &w, &h);
                std::cout << "Window resized to " << w << "x" << h << "\n";
            }
        }

        // --- Rendering ---
        SDL_SetRenderDrawColor(renderer, 25, 25, 90, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        // --- Small sleep to reduce CPU usage ---
        SDL_Delay(16);   // about 60 Hz
    }

    // --- Cleanup ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}