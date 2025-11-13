#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

int main(int argc, char* argv[]) 
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
    {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // Initialize SDL_ttf
    if (!TTF_Init()) 
    {
        std::cerr << "TTF_Init failed\n";
        SDL_Quit();
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL3 Window", 
                                          600, 600, 
                                          SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer (hardware accelerated)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load a font
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 48);
    if (!font) {
        std::cerr << "TTF_OpenFont failed\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Set draw color (RGBA) and clear the screen
        SDL_SetRenderDrawColor(renderer, 30, 30, 120, 255);
        SDL_RenderClear(renderer);

        // Render text
        SDL_Color color = {255, 255, 255, 255};
        const char* message = "Hello SDL3 + TTF!";
        size_t length = strlen(message); // Required in SDL3_ttf
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, length, color);

        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_DestroySurface(textSurface); // Use SDL_DestroySurface macro

            if (textTexture) {
                SDL_FRect dstRect = {50.f, 50.f, 400.f, 50.f}; // SDL3 uses SDL_FRect
                SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
        }

        // Present (swap buffers)
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
