#include "common.h"
#include "player.h"
#include "map.h"
#include "raycasting.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        exit(1);
    }

    playerX=280; playerY=350; playerDX = cos(playerAngle) *MOVEMENT_MODIFIER; playerDY = sin(playerAngle) *MOVEMENT_MODIFIER;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    init();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: wPressed = true; break;
                    case SDLK_s: sPressed = true; break;
                    case SDLK_a: aPressed = true; break;
                    case SDLK_d: dPressed = true; break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: wPressed = false; break;
                    case SDLK_s: sPressed = false; break;
                    case SDLK_a: aPressed = false; break;
                    case SDLK_d: dPressed = false; break;
                }
            }
        }

        updatePlayerPosition();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        drawMap2d();
        drawPlayer2d();
        drawRays2d();

        SDL_RenderPresent(renderer);
    }

    close();

    return 0;
}