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
#define N_TEXTURES_USED 4
#define TEXTURE_ID_FLOOR 0
#define TEXTURE_ID_WOLF 1
#define TEXTURE_ID_SPIDER 2
#define TEXTURE_ID_BEN 3
extern GLuint texture_names[N_TEXTURES_USED];
extern int flag_texture_mapping;

#define N_SPIDER_FRAMES 16
#define N_BEN_FRAMES 30
#define N_WOLF_FRAMES 17

void My_glTexImage2D_from_file(char* filename);
int read_geometry(GLfloat** object, int bytes_per_primitive, char* filename);

void prepare_spider(void);
void draw_spider(void);

void prepare_ben(void);
void draw_ben(void);

void prepare_wolf(void);
void draw_wolf(void);

void set_material_spider(void);
void set_material_ben(void);
void set_material_wolf(void);

