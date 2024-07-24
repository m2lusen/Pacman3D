#ifndef RAYCASTING_H
#define RAYCASTING_H

void rayHitMaths(float& rayX, float& rayY, float& rayAngle, float& xo, float& yo, float& distanceT, int& ray, int& mx, int& my, int& mp, int& dof, float originX, float originY, int& infLoopCheck, float& distanceP, int& verticalHorizontal);
void drawRays2d();


#endif // RAYCASTING_H