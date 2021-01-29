#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

// texture stuffs
#define N_TEXTURES_USED 7
#define TEXTURE_ID_FLOOR 0
#define TEXTURE_ID_WOLF 1
#define TEXTURE_ID_SPIDER 2
#define TEXTURE_ID_BEN 3
#define TEXTURE_ID_BUS 4
#define TEXTURE_ID_IRONMAN 5
#define TEXTURE_ID_BIKE 6
extern GLuint texture_names[N_TEXTURES_USED];
extern int flag_texture_mapping;

void My_glTexImage2D_from_file(char* filename);
int read_geometry(GLfloat** object, int bytes_per_primitive, char* filename);

void prepare_bus(void);
void draw_bus(void);
void cleanup_bus(void);

void prepare_bike(void);
void draw_bike(void);
void cleanup_bike(void);

void prepare_ironman(void);
void draw_ironman(void);
void cleanup_ironman(void);