#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

extern float playerX;
extern float playerY;
extern float playerDX;
extern float playerDY;
extern float playerAngle;
extern Uint32 prevFrameTime; 

void drawPlayer2d();
void updatePlayerPosition();

#endif
