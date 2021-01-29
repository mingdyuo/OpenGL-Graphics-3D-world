#pragma once
#include "initialize.h"
#include "floor.h"
#include "axis.h"
#include "Object.h"
#include "camera.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
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

#define N_SPIDER_FRAMES 16
#define N_BEN_FRAMES 30
#define N_WOLF_FRAMES 17

extern GLint loc_global_ambient_color;
extern loc_Material_Parameters loc_material;
extern GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
extern GLint loc_texture, loc_flag_texture_mapping, loc_flag_fog;

extern loc_light_Parameters loc_light[NUMBER_OF_LIGHT_SUPPORTED];
extern Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

extern int flag_fog;

// for tiger animation
extern unsigned int timestamp_scene; // the global clock in the scene
extern int flag_tiger_animation, flag_polygon_fill;
extern int cur_frame_spider, cur_frame_ben, cur_frame_wolf;
extern int rotation_angle;

extern GLuint h_ShaderProgram_simple, h_ShaderProgram_TXPS; // handles to shader programs

extern glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;
extern glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

extern float PRP_distance_scale[6];

extern GLuint texture_names[N_TEXTURES_USED];
extern int flag_texture_mapping;

extern float PRP;

void timer_scene(int value);
void keyboard(unsigned char key, int x, int y);

extern Camera camera;