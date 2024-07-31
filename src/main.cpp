#include "common.h"
#include "player.h"
#include "map.h"
#include "raycasting.h"
#include "enemy.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int score = 0;
bool boost = false;
float boostEndTime = 0;

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
    } // bellow causes crash, issue with TTF
    // if (TTF_Init() < 0) { // Initialize SDL TTF 
    //     std::cerr << "SDL TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
    //     SDL_DestroyRenderer(renderer);
    //     SDL_DestroyWindow(window);
    //     exit(1);
    // }

    playerX=280; playerY=350; playerDX = cos(playerAngle) *MOVEMENT_MODIFIER; playerDY = sin(playerAngle) *MOVEMENT_MODIFIER;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// BROKEN CAUSES CRASH
// void drawScore() { // move to its own file // issue with TTF

//     std::cout << "TEST " << std::endl;

    // SDL_Color textColor = {255, 255, 255, 255}; // White color
    // TTF_Font* font = TTF_OpenFont("../font/PixeloidSansBold-PKnYd.ttf", 24);
    // if (font == nullptr) {
    //     std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    //     return;
    // }
    // std::string scoreText = "Score: " + std::to_string(score);
    // SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    // if (textSurface == nullptr) {
    //     std::cerr << "Unable to render text surface: " << TTF_GetError() << std::endl;
    //     TTF_CloseFont(font);
    //     return;
    // }
    // SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    // if (textTexture == nullptr) {
    //     std::cerr << "Unable to create texture from rendered text: " << SDL_GetError() << std::endl;
    //     SDL_FreeSurface(textSurface);
    //     TTF_CloseFont(font);
    //     return;
    // }

    // std::cout << "TEST2 " << std::endl;

    // int textWidth = textSurface->w;
    // int textHeight = textSurface->h;
    // SDL_FreeSurface(textSurface);

    // SDL_Rect renderQuad = {10, 10, textWidth, textHeight};
    // SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    // SDL_DestroyTexture(textTexture);
    // TTF_CloseFont(font);
//}

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
        updateEnemies();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        drawMap2d();
        drawPlayer2d();

        drawEnemy2d();

        drawRays2d();

        // std::cout << "SCORE: " << score << std::endl; // TESTING
        // if (boost == true){
        //     std::cout << "Boost true" << std::endl; // TESTING
        // }


        SDL_RenderPresent(renderer);
    }

    close();

    return 0;
}
