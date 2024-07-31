#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <vector>

#define SCREEN_WIDTH 1160
#define SCREEN_HEIGHT 620
#define ANGLE_MODIFIER 2
#define MOVEMENT_MODIFIER 110

#define PI 3.1415926535
#define PID2 (PI / 2)
#define PI3D2 (3 * PI / 2)
#define DEGREE_FROM_RADIANS (PI / 180)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int score;
extern bool boost;
extern float boostEndTime;

extern bool wPressed;
extern bool sPressed;
extern bool aPressed;
extern bool dPressed;

#endif // COMMON_H