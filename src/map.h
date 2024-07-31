#ifndef MAP_H
#define MAP_H

#include <vector> 

extern std::vector<int> mapW;
extern std::vector<int> mapR;
extern int mapX, mapY, mapSize; 

void drawMap2d();
void checkAndPickupItem(float& playerX, float& playerY, int& score, bool& boost, float& boostEndTime);

#endif // MAP_H
