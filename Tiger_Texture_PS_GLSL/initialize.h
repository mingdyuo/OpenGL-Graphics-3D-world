#pragma once

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.

#include "floor.h"
#include "axis.h"
#include "static_objects.h"
#include "dynamic_objects.h"
#include "Object.h"

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