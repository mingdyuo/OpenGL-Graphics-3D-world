#include "floor.h"


// floor object
#define TEX_COORD_EXTENT 1.0f
GLuint rectangle_VBO, rectangle_VAO;
GLfloat rectangle_vertices[6][8] = {  // vertices enumerated counterclockwise
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, TEX_COORD_EXTENT, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, TEX_COORD_EXTENT, TEX_COORD_EXTENT },
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, TEX_COORD_EXTENT, TEX_COORD_EXTENT },
	{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, TEX_COORD_EXTENT }
};

Material_Parameters material_floor;
extern loc_Material_Parameters loc_material;
extern int floor_index;

void prepare_floor(void) { // Draw coordinate axes.
	// Initialize vertex buffer object.
	glGenBuffers(1, &rectangle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &rectangle_VAO);
	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_floor.ambient_color[0] = 0.0f;
	material_floor.ambient_color[1] = 0.05f;
	material_floor.ambient_color[2] = 0.0f;
	material_floor.ambient_color[3] = 1.0f;

	material_floor.diffuse_color[0] = 0.2f;
	material_floor.diffuse_color[1] = 0.5f;
	material_floor.diffuse_color[2] = 0.2f;
	material_floor.diffuse_color[3] = 1.0f;

	material_floor.specular_color[0] = 0.24f;
	material_floor.specular_color[1] = 0.5f;
	material_floor.specular_color[2] = 0.24f;
	material_floor.specular_color[3] = 1.0f;

	material_floor.specular_exponent = 2.5f;

	material_floor.emissive_color[0] = 0.0f;
	material_floor.emissive_color[1] = 0.0f;
	material_floor.emissive_color[2] = 0.0f;
	material_floor.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_FLOOR);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_FLOOR]);

		 //My_glTexImage2D_from_file("Data/barcode.jpg");
	My_glTexImage2D_from_file("Data/fussballfeld_03_c.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 	 float border_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	// 	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// 	 glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

void set_material_floor(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_floor.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_floor.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_floor.specular_color);
	glUniform1f(loc_material.specular_exponent, material_floor.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_floor.emissive_color);
}

void draw_floor(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void cleanup_floor(void) {
	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);
}



Material_Parameters material_floor2;
extern loc_Material_Parameters loc_material;
extern int floor2_index;

void prepare_floor2(void) { // Draw coordinate axes.
	// Initialize vertex buffer object.
	glGenBuffers(1, &rectangle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &rectangle_VAO);
	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_floor2.ambient_color[0] = 0.0f;
	material_floor2.ambient_color[1] = 0.05f;
	material_floor2.ambient_color[2] = 0.0f;
	material_floor2.ambient_color[3] = 1.0f;

	material_floor2.diffuse_color[0] = 0.2f;
	material_floor2.diffuse_color[1] = 0.5f;
	material_floor2.diffuse_color[2] = 0.2f;
	material_floor2.diffuse_color[3] = 1.0f;

	material_floor2.specular_color[0] = 0.24f;
	material_floor2.specular_color[1] = 0.5f;
	material_floor2.specular_color[2] = 0.24f;
	material_floor2.specular_color[3] = 1.0f;

	material_floor2.specular_exponent = 2.5f;

	material_floor2.emissive_color[0] = 0.0f;
	material_floor2.emissive_color[1] = 0.0f;
	material_floor2.emissive_color[2] = 0.0f;
	material_floor2.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_FLOOR2);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_FLOOR2]);

	//My_glTexImage2D_from_file("Data/barcode.jpg");
	My_glTexImage2D_from_file("Data/sky.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 	 float border_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	// 	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// 	 glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

void set_material_floor2(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_floor2.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_floor2.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_floor2.specular_color);
	glUniform1f(loc_material.specular_exponent, material_floor2.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_floor2.emissive_color);
}

void draw_floor2(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void cleanup_floor2(void) {
	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);
}