#include "enemy.h"
#include "player.h"
#include "common.h"
#include "map.h"

enum EnemyState { CHASE, SCATTER, SCARED, EATEN }; // Define the enemy states

enum EnemyDirection { NORTH, EAST, SOUTH, WEST };

// the following is a placeholder, in future copy actual packman chase and scatter (4 chases and scatters per level)
int scatterTime = 7;
int chaseTime = 20;

auto getNewDirection = [](int currentDirection, int turn) {
    return (currentDirection + turn + 360) % 360;
};

auto getNewPosition = [](int x, int y, EnemyDirection direction) {
    switch (direction) {
        case NORTH: return std::make_pair(x, y - 1); // North
        case EAST: return std::make_pair(x + 1, y); // East
        case SOUTH: return std::make_pair(x, y + 1); // South
        case WEST: return std::make_pair(x - 1, y); // West
    }
};

struct Enemy { // Structure for an enemy // move to .h
    std::string name;
    SDL_Color color;
    int startX, startY;
    float x, y;
    int targetX, targetY;
    EnemyState state;
    int startTime;
    EnemyDirection direction;
};

void drawCircle(int x, int y, int radius, SDL_Color color) { // should include in a common file, could be shared with power ups, and player for 2d map
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

std::vector<Enemy> enemies = { // update times to match pacman, placeholder for now
    {"Blinky", {255, 0, 0, 255}, 260, 220, 260, 220, 260, 220, SCATTER, 0, NORTH}, // not central to the centre of the screen but central to the grid
    {"Pinky", {255, 192, 203, 255}, 220, 280, 220, 280, 220, 280, SCATTER, 5, NORTH}, // should start waiting, add extra state or use eaten
    {"Inky", {0, 255, 255, 255}, 260, 280, 260, 280, 260, 280, SCATTER, 10, NORTH}, // should start waiting, add extra state or use eaten
    {"Clyde", {255, 165, 0, 255}, 300, 280, 300, 280, 300, 280, SCATTER, 15, NORTH}  // should start waiting, add extra state or use eaten
};

void updateEnemyState(Enemy& enemy) { // not working correctly
    Uint32 currentTime = SDL_GetTicks() / 1000;
    int elapsedTime = currentTime - enemy.startTime;
    if (elapsedTime > chaseTime + scatterTime) {
        enemy.startTime = currentTime;
        elapsedTime = 0;
    }

    if (elapsedTime < scatterTime) { // temporary in future make sure doesnt effect scared or eaten
        enemy.state = SCATTER;
    } else {
        enemy.state = CHASE;
    }
}

void enemyMove(Enemy& enemy, int targetX, int targetY){ // enemy can go backwards, speed is inconsistent (believe the issue is to do with the speed from differenty states stacking), teleportation over borders not implented
    if (enemy.x != enemy.targetX || enemy.y != enemy.targetY ){
        float speed = 0.125; // Use integer speed for consistent movement

        if (enemy.x < enemy.targetX) enemy.x += speed;
        else if (enemy.x > enemy.targetX) enemy.x -= speed;
        if (enemy.y < enemy.targetY) enemy.y += speed;
        else if (enemy.y > enemy.targetY) enemy.y -= speed;
    } else { // not working correctly
        std::vector<EnemyDirection> directionsOrder;
        std::vector<std::pair<int, int>> directionsVectors;
        switch (enemy.direction) {
            case NORTH:
                directionsVectors = {{0, -20}, {-20, 0}, {20, 0}}; // Forward, left, right
                directionsOrder = {NORTH, WEST, EAST};
                break;
            case EAST: 
                directionsVectors = {{20, 0}, {0, -20}, {0, 20}}; // Forward, left, right
                directionsOrder = {EAST, NORTH, SOUTH};
                break;
            case SOUTH: 
                directionsVectors = {{0, 20}, {20, 0}, {-20, 0}}; // Forward, left, right
                directionsOrder = {SOUTH, EAST, WEST};
                break;
            case WEST: 
                directionsVectors = {{-20, 0}, {0, 20}, {0, -20}}; // Forward, left, right
                directionsOrder = {WEST, SOUTH, NORTH};
                break;
        }

        std::pair<int, int> bestMove = {enemy.x, enemy.y};
        int minDistance = 727379969;
        EnemyDirection bestDirection;
        
        for (size_t i = 0; i < directionsVectors.size(); ++i){
            int newX = enemy.x + directionsVectors[i].first;
            int newY = enemy.y + directionsVectors[i].second;

            if (newX == 0) newX = 540;
            else if (newX == 540) newX = 0;

            if (mapW[(newY / 20) * mapX + (newX  /20)] == 0) {
                int distance = std::abs(targetX - newX) + std::abs(targetY - newY);
                if (distance < minDistance) { 
                    minDistance = distance;
                    bestMove = {newX, newY};
                    
                    bestDirection = directionsOrder[i];
                }
            }
        }

        enemy.direction = bestDirection;
        enemy.targetX = bestMove.first;
        enemy.targetY = bestMove.second;
    }

    if (enemy.x <= 0) enemy.x = 540;
    else if (enemy.x >= 540) enemy.x = 0;
}

void updateEnemyPosition(Enemy& enemy) { // blinky speed not constant
    updateEnemyState(enemy);
    Uint32 currentTime = SDL_GetTicks() / 1000;

    switch (enemy.state) { // believe that the different states causes issues with movement speed
        case CHASE: { 
            enemyMove(enemy, playerX, playerY);
        }
        case SCATTER: { // reorganise the following
            if (enemy.name == "Blinky") {
                int targetX = mapSize * (mapX - 1.5);
                int targetY = 20;
                enemyMove(enemy, targetX, targetY);
            } else if (enemy.name == "Inky" && enemy.startTime <= currentTime) {
                int targetX = 20;
                int targetY = 20;
                enemyMove(enemy, targetX, targetY);
            } else if (enemy.name == "Pinky" && enemy.startTime <= currentTime) {
                int targetX = mapSize * (mapX - 1.5);
                int targetY = mapSize * (mapY - 1.5);
                enemyMove(enemy, targetX, targetY);
            } else if (enemy.name == "Clyde" && enemy.startTime <= currentTime) {
                int targetX = 20;
                int targetY = mapSize * (mapY - 1.5);
                enemyMove(enemy, targetX, targetY);
            }
            break;
        }
        // case SCARED: { // temp
        //     if (playerX > enemy.x) enemy.x--;
        //     else if (playerX < enemy.x) enemy.x++;
        //     if (playerY > enemy.y) enemy.y--;
        //     else if (playerY < enemy.y) enemy.y++;
        //     break;
        // }
        // case EATEN: {
        //     if (enemy.x != enemy.startX) enemy.x += (enemy.startX - enemy.x) / std::abs(enemy.startX - enemy.x);
        //     if (enemy.y != enemy.startY) enemy.y += (enemy.startY - enemy.y) / std::abs(enemy.startY - enemy.y);
        //     break;
        // }
    }
}


void updateEnemies() {
    for (auto& enemy : enemies) {
        updateEnemyPosition(enemy);
    }
}

void drawEnemy2d() {
    for (auto& enemy : enemies) {
        drawCircle(enemy.x + 10, enemy.y + 10, 10, enemy.color);
    }
}