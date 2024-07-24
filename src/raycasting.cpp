#include "raycasting.h"
#include "common.h"
#include "player.h"
#include "map.h"

float distance(float ax, float ay, float bx, float by) {
    return std::sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

void rayHitMaths(float& rayX, float& rayY, float& rayAngle, float& xo, float& yo, float& distanceT, int& ray, int& mx, int& my, int& mp, int& dof, float originX, float originY, int& infLoopCheck, float& distanceP, int& verticalHorizontal) {
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
            	rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, newX, portalY, infLoopCheck, distanceP, verticalHorizontal);
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
            	rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, newX, portalY, infLoopCheck, distanceP, verticalHorizontal);
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
		   verticalHorizontal = 0; // horizontal wall
		}
		if (distanceH < distanceV) {
			rayX = horizontalX;
			rayY = horizontalY;
			distanceT=(distanceH + distanceP) * cos(playerAngle-rayAngle);
			verticalHorizontal = 1; // vertical wall
		}
	}

}

void drawRays2d() {
    int ray, mx, my, mp, dof, infLoopCheck = 0, verticalHorizontal;
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
        rayHitMaths(rayX, rayY, rayAngle, xo, yo, distanceT, ray, mx, my, mp, dof, originX, originY, infLoopCheck, distanceP, verticalHorizontal);

        // Draw rays
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for rays
        SDL_RenderDrawLine(renderer, static_cast<int>(playerX), static_cast<int>(playerY),
                           static_cast<int>(rayX), static_cast<int>(rayY));

        // Draw 3D projection
		float cAngle = playerAngle - rayAngle;
		if (cAngle < 0) cAngle += 2 * PI;
		if (cAngle > 2 * PI) cAngle -= 2 * PI; // Calculate the cosine angle to prevent fish-eye effect
		float lineH = (mapSize * 600) / distanceT; // Calculate the corrected line height
		if (lineH > 600) lineH = 600;
		float lineO = 300 - lineH / 2; // Calculate the line offset

		if (verticalHorizontal == 0){ // Set the color 
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, 0, 0, 230, 255);
		}

		int leftX = ray * 4 + (mapX * mapSize) - 4;
		int rightX = ray * 4 + (mapX * mapSize) + 4;
		SDL_Rect rect = {leftX, static_cast<int>(lineO), rightX - leftX, static_cast<int>(lineH)};
		SDL_RenderFillRect(renderer, &rect);

        rayAngle += DEGREE_FROM_RADIANS / 2; // Increase angle
        if (rayAngle < 0) {
            rayAngle += 2 * PI;
        }
        if (rayAngle > 2 * PI) {
            rayAngle -= 2 * PI;
        }
    }
}