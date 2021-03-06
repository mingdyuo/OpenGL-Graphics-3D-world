#pragma once
#include "initialize.h"
#include "floor.h"
#include "axis.h"
#include "camera.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.

// texture stuffs
#define N_TEXTURES_USED 9
#define TEXTURE_ID_FLOOR 0
#define TEXTURE_ID_FLOOR2 7
#define TEXTURE_ID_WOLF 1
#define TEXTURE_ID_SPIDER 2
#define TEXTURE_ID_BEN 3
#define TEXTURE_ID_BUS 4
#define TEXTURE_ID_IRONMAN 5
#define TEXTURE_ID_IRONMAN2 8
#define TEXTURE_ID_BIKE 6


#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

#define NUMBER_OF_LIGHT_SUPPORTED 4 

void timer_scene(int value);
void keyboard(unsigned char key, int x, int y);
//void init_objects();

