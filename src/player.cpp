#include "player.h"
#include "common.h"
#include "map.h"

float playerX = 0, playerY = 0, playerDX = 0, playerDY = 0, playerAngle = 0; // Player position
bool wPressed = false, sPressed = false, aPressed = false, dPressed = false; // Key presses
Uint32 prevFrameTime = 0;

void drawPlayer2d() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
    // Draw player as a circle
    const int radius = 5;
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, static_cast<int>(playerX) + dx, static_cast<int>(playerY) + dy);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_RenderDrawLine(renderer, static_cast<int>(playerX), static_cast<int>(playerY),
                       static_cast<int>(playerX + playerDX), static_cast<int>(playerY + playerDY));
}

void updatePlayerPosition() {
    Uint32 currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - prevFrameTime) / 1000.0f; // Convert to seconds
    prevFrameTime = currentFrameTime;

    if (aPressed) {
        playerAngle -= ANGLE_MODIFIER * deltaTime;
        if (playerAngle < 0) playerAngle += 2 * PI;
        playerDX = std::cos(playerAngle) * MOVEMENT_MODIFIER;
        playerDY = std::sin(playerAngle) * MOVEMENT_MODIFIER;
    }
    if (dPressed) {
        playerAngle += ANGLE_MODIFIER * deltaTime;
        if (playerAngle > 2 * PI) playerAngle -= 2 * PI;
        playerDX = std::cos(playerAngle) * MOVEMENT_MODIFIER;
        playerDY = std::sin(playerAngle) * MOVEMENT_MODIFIER;
    }

    int xOffset = (playerDX < 0) ? -10 : 10;
    int yOffset = (playerDY < 0) ? -10 : 10;
    int initialPlayerX = static_cast<int>(playerX) / mapSize;
    int initialPlayerY = static_cast<int>(playerY) / mapSize;
    int initialPlayerX_add_xOffset = (playerX + xOffset) / mapSize;
    int initialPlayerY_add_yOffset = (playerY + yOffset) / mapSize;
    int initialPlayerX_sub_xOffset = (playerX - xOffset) / mapSize;
    int initialPlayerY_sub_yOffset = (playerY - yOffset) / mapSize;

    if (wPressed) {
        if (mapW[initialPlayerY * mapX + initialPlayerX_add_xOffset] == 0) {
            playerX += playerDX * deltaTime;
        }
        if (mapW[initialPlayerY_add_yOffset * mapX + initialPlayerX] == 0) {
            playerY += playerDY * deltaTime;
        }
    }
    if (sPressed) {
        if (mapW[initialPlayerY * mapX + initialPlayerX_sub_xOffset] == 0) {
            playerX -= playerDX * deltaTime;
        }
        if (mapW[initialPlayerY_sub_yOffset * mapX + initialPlayerX] == 0) {
            playerY -= playerDY * deltaTime;
        }
    }

    int currentSquare = initialPlayerY * mapX + initialPlayerX;
    if (currentSquare == 392 && playerX <= 20) {
        playerX = 539;
    } else if (currentSquare == 419 && playerX >= 530) {
        playerX = 21;
    }
}