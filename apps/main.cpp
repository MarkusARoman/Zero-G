#include <SDL3/SDL.h>
#include <cmath>
#include <iostream>

class Camera {
public:
    float x = 0.0f;
    float y = 0.0f;
    float zoom = 1.0f;
    bool dragging = false;
    int lastMouseX = 0, lastMouseY = 0;

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
            dragging = true;
            lastMouseX = e.button.x;
            lastMouseY = e.button.y;
        } 
        else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT) {
            dragging = false;
        } 
        else if (e.type == SDL_EVENT_MOUSE_MOTION && dragging) {
            int dx = e.motion.x - lastMouseX;
            int dy = e.motion.y - lastMouseY;
            x -= dx / zoom;
            y -= dy / zoom;
            lastMouseX = e.motion.x;
            lastMouseY = e.motion.y;
        }
        else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            if (e.wheel.y > 0) zoom *= 1.1f;
            if (e.wheel.y < 0) zoom /= 1.1f;
            if (zoom < 0.1f) zoom = 0.1f;
            if (zoom > 5.0f) zoom = 5.0f;
        }
    }

    // Convert world coordinates to screen coordinates
    SDL_FPoint worldToScreen(float wx, float wy, int screenW, int screenH) const {
        SDL_FPoint p;
        p.x = (screenW / 2.0f) + (wx - x) * zoom;
        p.y = (screenH / 2.0f) + (wy - y) * zoom;
        return p;
    }
};


class Projectile {
public:
    float x = 0, y = 0;
    float vx = 0, vy = 0;
    bool active = false;

    void update(float dt) {
        if (!active) return;
        vy += 500.0f * dt; // gravity
        x += vx * dt;
        y += vy * dt;

        // ground collision
        if (y > 0) {
            y = 0;
            vy = 0;
            vx = 0;
            active = false;
        }
    }

    void draw(SDL_Renderer* renderer, const Camera& cam, int screenW, int screenH) {
        if (!active) return;
        SDL_FPoint screenPos = cam.worldToScreen(x, y, screenW, screenH);
        SDL_FRect rect = { screenPos.x - 4, screenPos.y - 4, 8, 8 };
        SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};


class Launcher {
public:
    float x = 0, y = 0;
    float angle = -0.5f;  // radians
    float power = 300.0f;

    virtual ~Launcher() = default;
    virtual void update(float dt) = 0;
    virtual void draw(SDL_Renderer* renderer, const Camera& cam, int screenW, int screenH) = 0;
    virtual Projectile launch() = 0;
};


class Cannon : public Launcher {
public:
    void update(float dt) override {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_LEFT])  angle -= 1.5f * dt;
        if (keys[SDL_SCANCODE_RIGHT]) angle += 1.5f * dt;
        if (keys[SDL_SCANCODE_UP])    power += 100.0f * dt;
        if (keys[SDL_SCANCODE_DOWN])  power -= 100.0f * dt;

        if (power < 50.0f) power = 50.0f;
        if (power > 800.0f) power = 800.0f;
    }

    void draw(SDL_Renderer* renderer, const Camera& cam, int screenW, int screenH) override {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FPoint base = cam.worldToScreen(x, y, screenW, screenH);
        float endX = x + std::cos(angle) * 40.0f;
        float endY = y + std::sin(angle) * 40.0f;
        SDL_FPoint end = cam.worldToScreen(endX, endY, screenW, screenH);
        SDL_RenderLine(renderer, base.x, base.y, end.x, end.y);
    }

    Projectile launch() override {
        Projectile p;
        p.x = x;
        p.y = y;
        p.vx = std::cos(angle) * power;
        p.vy = std::sin(angle) * power;
        p.active = true;
        return p;
    }
};


int main() 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Zero-G Prototype with Camera", 1280, 720, SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Cannon cannon;
    Projectile projectile;
    Camera camera;

    bool running = true;
    Uint64 last = SDL_GetTicks();

    while (running) {
        Uint64 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        int screenW, screenH;
        SDL_GetWindowSize(window, &screenW, &screenH);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) running = false;
                if (e.key.key == SDLK_SPACE && !projectile.active) {
                    projectile = cannon.launch();
                }
            }
            camera.handleEvent(e);
        }

        cannon.update(dt);
        projectile.update(dt);

        SDL_SetRenderDrawColor(renderer, 30, 40, 50, 255);
        SDL_RenderClear(renderer);

        cannon.draw(renderer, camera, screenW, screenH);
        projectile.draw(renderer, camera, screenW, screenH);

        // Draw origin crosshair (0,0)
        SDL_FPoint origin = camera.worldToScreen(0, 0, screenW, screenH);
        SDL_SetRenderDrawColor(renderer, 200, 80, 80, 255);
        SDL_RenderLine(renderer, origin.x - 10, origin.y, origin.x + 10, origin.y);
        SDL_RenderLine(renderer, origin.x, origin.y - 10, origin.x, origin.y + 10);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
