#pragma once

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.

#include "floor.h"
#include "axis.h"
#include "Object.h"
#include "camera.h"
#include "externs.h"
#include <vector>

void prepare_shader_program(void);
void initialize_flags(void);
void initialize_OpenGL(void);
void initialize_lights_and_material(void);
void prepare_scene(void);
void initialize_renderer(void);

void register_callbacks(void);
//void prepare_tiger(void);

void initialize_glew(void);

extern Object bus;
extern Object bike;
extern Object ironman;
extern Dynamic_Obj spider;
extern Dynamic_Obj wolf;
extern Dynamic_Obj ben;