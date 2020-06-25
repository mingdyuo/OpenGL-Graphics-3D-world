#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

int read_geometry(GLfloat** object, int bytes_per_primitive, char* filename);
void My_glTexImage2D_from_file(char* filename);