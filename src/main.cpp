#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#define SCREEN_WIDTH 1160
#define SCREEN_HEIGHT 620
#define ANGLE_MODIFIER 2
#define MOVEMENT_MODIFIER 100

#define PI 3.1415926535
#define PID2 (PI / 2)
#define PI3D2 (3 * PI / 2)
#define DEGREE_FROM_RADIANS (PI / 180)

const int mapX = 28;
const int mapY = 31;
const int mapSize = 20;

std::vector<int> mapW = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
	1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
	1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
	1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
	1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,
	0,0,0,0,0,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,
	0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,
	0,0,0,0,0,1,0,1,1,0,1,1,1,2,2,1,1,1,0,1,1,0,1,0,0,0,0,0,
	1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1,
	0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,
	0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,
	0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,
	1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
	1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
	1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,
	1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,
	1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,
	1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,
	1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
	1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

std::vector<int> mapR = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
	0,2,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,2,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,
	0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,
	0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
	0,2,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,2,0,
	0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,
	0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,
	0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,
	0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};


float playerX = 0, playerY = 0, playerDX = 0, playerDY = 0, playerAngle = 0; // Player position

bool wPressed = false, sPressed = false, aPressed = false, dPressed = false; // Key presses

Uint32 prevFrameTime = 0;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

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

void drawMap2d() {
    for (int y = 0; y < mapY; ++y) {
        for (int x = 0; x < mapX; ++x) {
            SDL_Rect tileRect = { x * mapSize, y * mapSize, mapSize, mapSize };
            if (mapW[y * mapX + x] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
            }
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}

float distance(float ax, float ay, float bx, float by) {
    return std::sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

// fix based on original C

void rayHitMaths(float& rayX, float& rayY, float& rayAngle, float& xo, float& yo, float& distanceT, int& ray, int& mx, int& my, int& mp, int& dof, float originX, float originY, int& infLoopCheck, float& distanceP) {
    float portalX, portalY, portalA;
    int recursiveCallCheck = 0;

    // Horizontal
    dof = 0;
    float distanceH = 99999, horizontalX = originX, horizontalY = originY;
    float aTan = -1 / tan(rayAngle);
    if (rayAngle > PI) {
        rayY = (((int)originY / mapSize) * mapSize) - 0.0001;
        rayX = (originY - rayY) * aTan + originX;
        yo = -mapSize;
        xo = -yo * aTan;
    }
    if (rayAngle < PI) {
        rayY = (((int)originY / mapSize) * mapSize) + mapSize;
        rayX = (originY - rayY) * aTan + originX;
        yo = mapSize;
        xo = -yo * aTan;
    }
    if (rayAngle == 0 || rayAngle == PI) { // looking straight left or right
        rayX = originX;
        rayY = originY;
        dof = 28;
    }

    while (dof < 28) {
        mx = (int)(rayX) / mapSize;
        my = (int)(rayY) / mapSize;
        mp = my * mapX + mx;
        if (mp > 0 && mp < mapX * mapY && mapW[mp] == 1) { // double check correct
            horizontalX = rayX;
            horizontalY = rayY;
            distanceH = distance(originX, originY, horizontalX, horizontalY);
            dof = 28;
        } else {
            rayX += xo;
            rayY += yo;
            (dof)++;
        }
    }

    // Vertical ray calculation
	dof = 0;
    float distanceV = 99999, verticalX = originX, verticalY = originY;
    float nTan = -tan(rayAngle);
    if (rayAngle > PID2 && rayAngle < PI3D2) {
        rayX = (((int)originX / mapSize) * mapSize) - 0.0001;
        rayY = (originX - rayX) * nTan + originY;
        xo = -mapSize;
        yo = -xo * nTan;
    }
    if (rayAngle < PID2 || rayAngle > PI3D2) {
        rayX = (((int)originX / mapSize) * mapSize) + mapSize;
        rayY = (originX - rayX) * nTan + originY;
        xo = mapSize;
        yo = -xo * nTan;
    }
    if (rayAngle == 0 || rayAngle == PI) {
        rayX = originX;
        rayY = originY;
        dof = 28;
    }
    while (dof < 28) {
        mx = (int)(rayX) / mapSize;
        my = (int)(rayY) / mapSize;
        mp = my * mapX + mx;
        if (mp > 0 && mp < mapX * mapY && mapW[mp] == 1) {
            verticalX = rayX;
            verticalY = rayY;
            distanceV = distance(originX, originY, verticalX, verticalY);
            dof = 28;
        } else {
            rayX += xo;
            rayY += yo;
            dof++;
        }

		if (mp == 392) {
            portalX = rayX;
            portalY = rayY;
            portalA = rayAngle;            
            if (infLoopCheck==0){
            	recursiveCallCheck = 1;
            	infLoopCheck+=1;
            	float newX = 531;            	
            	float distanceP =  distance(originX, originY, rayX, rayY);            	
            	rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, newX, portalY, infLoopCheck, distanceP);
            }
        } else if (mp == 419){
        	portalX = rayX;
            portalY = rayY;
            portalA = rayAngle;
            if (infLoopCheck==0){
            	recursiveCallCheck = 1;
            	infLoopCheck+=1;
            	float newX = 21;
            	float distanceP =  distance(originX, originY, rayX, rayY);
            	rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, newX, portalY, infLoopCheck, distanceP);
            }
		}
    }

    if (recursiveCallCheck == 0) {
        if (distanceH < distanceV) {
            distanceP = distanceH;
        } else {
            distanceP = distanceV;
        }
    }

	if (recursiveCallCheck == 0){
		if (distanceV < distanceH) {
		   rayX = verticalX;
		   rayY = verticalY;
		   distanceT=(distanceV + distanceP) * cos(playerAngle-rayAngle);
		}
		if (distanceH < distanceV) {
			rayX = horizontalX;
			rayY = horizontalY;
			distanceT=(distanceH + distanceP) * cos(playerAngle-rayAngle);
		}
	}

}

void drawRays2d() {
    int ray, mx, my, mp, dof, infLoopCheck = 0;
    float rayX, rayY, rayAngle, xo, yo, distanceT = 0, distanceP = 0;
    float originX = playerX, originY = playerY;
    rayAngle = playerAngle - DEGREE_FROM_RADIANS * 30;
    if (rayAngle < 0) {
        rayAngle += 2 * PI;
    }
    if (rayAngle > 2 * PI) {
        rayAngle -= 2 * PI;
    }

    for (ray = 0; ray < 120; ray++) {
        rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, originX, originY, infLoopCheck, distanceP);

        // Draw rays
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for rays
        SDL_RenderDrawLine(renderer, static_cast<int>(playerX), static_cast<int>(playerY),
                           static_cast<int>(rayX), static_cast<int>(rayY));

        // Draw 3D projection

        float cAngle = playerAngle - rayAngle; // cosine angle, used to stop fish-eye effect
        if (cAngle < 0) cAngle += 2 * PI;
        if (cAngle > 2 * PI) cAngle -= 2 * PI;
        float lineH = (mapSize * 600) / distanceT; // line height
        if (lineH > 600) lineH = 600;
        float lineO = (300) - lineH / 2; // line Offset
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for 3D projection
        SDL_RenderDrawLine(renderer, ray * 4 + (mapX * mapSize), static_cast<int>(lineO),
                           ray * 4 + (mapX * mapSize), static_cast<int>(lineH + lineO));

        rayAngle += DEGREE_FROM_RADIANS / 2; // Increase angle
        if (rayAngle < 0) {
            rayAngle += 2 * PI;
        }
        if (rayAngle > 2 * PI) {
            rayAngle -= 2 * PI;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("2D Map and Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	playerX=280; playerY=350; playerDX = cos(playerAngle) *MOVEMENT_MODIFIER; playerDY = sin(playerAngle) *MOVEMENT_MODIFIER;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w) wPressed = true;
                if (event.key.keysym.sym == SDLK_s) sPressed = true;
                if (event.key.keysym.sym == SDLK_a) aPressed = true;
                if (event.key.keysym.sym == SDLK_d) dPressed = true;
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_w) wPressed = false;
                if (event.key.keysym.sym == SDLK_s) sPressed = false;
                if (event.key.keysym.sym == SDLK_a) aPressed = false;
                if (event.key.keysym.sym == SDLK_d) dPressed = false;
            }
        }

        updatePlayerPosition();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        drawMap2d();
        drawPlayer2d();
        drawRays2d(); // Draw rays

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Roughly 60 frames per second
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
