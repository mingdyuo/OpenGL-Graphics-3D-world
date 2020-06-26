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

extern GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
extern glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;
extern glm::mat4 ViewMatrix, ProjectionMatrix;

void My_glTexImage2D_from_file(char* filename);
int read_geometry(GLfloat** object, int bytes_per_primitive, char* filename);

enum OBJ_TYPE {
	OBJ_BUS,
	OBJ_BIKE,
	OBJ_IRONMAN,
	OBJ_WOLF,
	OBJ_BEN,
	OBJ_SPIDER

};

class Object {
private:
public:
	GLuint obj_VBO, obj_VAO;
	int obj_n_triangles;
	GLfloat* obj_vertices;
	std::string obj_filename;
	Material_Parameters material_obj;
	OBJ_TYPE obj_type;

	Object(OBJ_TYPE type);
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 model_rotate_axis;
	glm::vec3 world_rotate_axis;
	float model_rotate_angle;
	float world_rotate_angle;


	void draw();
	void prepare();
	void set_material();
	

};

void color_bus(Material_Parameters& material_obj);
void color_bike(Material_Parameters& material_obj);
void color_ironman(Material_Parameters& material_obj);

//class Dynamic_Obj : public Object {
//public:
//	int obj_n_triangles;
//	GLfloat* obj_vertices;
//
//	int* obj_cur_frame;
//	void draw();
//	Dynamic_Obj(OBJ_TYPE type, int* cur_frame);
//
//};