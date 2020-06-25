#pragma once
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/matrix_inverse.hpp> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
#include "file_reading_function.h"
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
extern loc_Material_Parameters loc_material;

enum OBJ_TYPE {
	OBJ_BUS,
	OBJ_BIKE
};

class Object {
private:
	GLuint obj_VBO, obj_VAO;
	int obj_n_triangles;
	GLfloat* obj_vertices;
	std::string obj_filename;
	Material_Parameters material_obj;
	OBJ_TYPE obj_type;
public:
	Object(std::string filename, OBJ_TYPE type);
	void draw();
	void prepare();
	void set_material();


};

void color_bus(Material_Parameters& material_obj);
void color_bike(Material_Parameters& material_obj);