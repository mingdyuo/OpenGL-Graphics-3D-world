#define _CRT_SECURE_NO_WARNINGS
#include "static_objects.h"


// bus object
GLuint bus_VBO, bus_VAO;
int bus_n_triangles;
GLfloat* bus_vertices;

Material_Parameters material_bus;

void prepare_bus(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, bus_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/bus_vnt.geom");
	bus_n_triangles = read_geometry(&bus_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	bus_n_total_triangles += bus_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &bus_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, bus_VBO);
	glBufferData(GL_ARRAY_BUFFER, bus_n_total_triangles * 3 * n_bytes_per_vertex, bus_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(bus_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &bus_VAO);
	glBindVertexArray(bus_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, bus_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_bus.ambient_color[0] = 0.24725f;
	material_bus.ambient_color[1] = 0.1995f;
	material_bus.ambient_color[2] = 0.0745f;
	material_bus.ambient_color[3] = 1.0f;
	
	material_bus.diffuse_color[0] = 0.75164f;
	material_bus.diffuse_color[1] = 0.60648f;
	material_bus.diffuse_color[2] = 0.22648f;
	material_bus.diffuse_color[3] = 1.0f;
	
	material_bus.specular_color[0] = 0.728281f;
	material_bus.specular_color[1] = 0.655802f;
	material_bus.specular_color[2] = 0.466065f;
	material_bus.specular_color[3] = 1.0f;
	
	material_bus.specular_exponent = 51.2f;
	
	material_bus.emissive_color[0] = 0.1f;
	material_bus.emissive_color[1] = 0.1f;
	material_bus.emissive_color[2] = 0.0f;
	material_bus.emissive_color[3] = 1.0f;

	/*glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_TIGER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/tiger/tiger_tex2.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
}

void draw_bus(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(bus_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * bus_n_triangles);
	glBindVertexArray(0);
}

void cleanup_bus(void) {
	glDeleteVertexArrays(1, &bus_VAO);
	glDeleteBuffers(1, &bus_VBO);
}

// bike object
GLuint bike_VBO, bike_VAO;
int bike_n_triangles;
GLfloat* bike_vertices;

Material_Parameters material_bike;

void prepare_bike(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, bike_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/bike_vnt.geom");
	bike_n_triangles = read_geometry(&bike_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	bike_n_total_triangles += bike_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &bike_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, bike_VBO);
	glBufferData(GL_ARRAY_BUFFER, bike_n_total_triangles * 3 * n_bytes_per_vertex, bike_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(bike_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &bike_VAO);
	glBindVertexArray(bike_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, bike_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_bike.ambient_color[0] = 0.24725f;
	material_bike.ambient_color[1] = 0.1995f;
	material_bike.ambient_color[2] = 0.0745f;
	material_bike.ambient_color[3] = 1.0f;
	
	material_bike.diffuse_color[0] = 0.75164f;
	material_bike.diffuse_color[1] = 0.60648f;
	material_bike.diffuse_color[2] = 0.22648f;
	material_bike.diffuse_color[3] = 1.0f;
	
	material_bike.specular_color[0] = 0.728281f;
	material_bike.specular_color[1] = 0.655802f;
	material_bike.specular_color[2] = 0.466065f;
	material_bike.specular_color[3] = 1.0f;
	
	material_bike.specular_exponent = 51.2f;
	
	material_bike.emissive_color[0] = 0.1f;
	material_bike.emissive_color[1] = 0.1f;
	material_bike.emissive_color[2] = 0.0f;
	material_bike.emissive_color[3] = 1.0f;

	/*glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_TIGER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/tiger/tiger_tex2.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
}

void draw_bike(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(bike_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * bike_n_triangles);
	glBindVertexArray(0);
}

void cleanup_bike(void) {
	glDeleteVertexArrays(1, &bike_VAO);
	glDeleteBuffers(1, &bike_VBO);
}


// ironman object
GLuint ironman_VBO, ironman_VAO;
int ironman_n_triangles;
GLfloat* ironman_vertices;

Material_Parameters material_ironman;

void prepare_ironman(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, ironman_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/ironman_vnt.geom");
	ironman_n_triangles = read_geometry(&ironman_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	ironman_n_total_triangles += ironman_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &ironman_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glBufferData(GL_ARRAY_BUFFER, ironman_n_total_triangles * 3 * n_bytes_per_vertex, ironman_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(ironman_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &ironman_VAO);
	glBindVertexArray(ironman_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_ironman.ambient_color[0] = 0.24725f;
	material_ironman.ambient_color[1] = 0.1995f;
	material_ironman.ambient_color[2] = 0.0745f;
	material_ironman.ambient_color[3] = 1.0f;
	
	material_ironman.diffuse_color[0] = 0.75164f;
	material_ironman.diffuse_color[1] = 0.60648f;
	material_ironman.diffuse_color[2] = 0.22648f;
	material_ironman.diffuse_color[3] = 1.0f;
	
	material_ironman.specular_color[0] = 0.728281f;
	material_ironman.specular_color[1] = 0.655802f;
	material_ironman.specular_color[2] = 0.466065f;
	material_ironman.specular_color[3] = 1.0f;
	
	material_ironman.specular_exponent = 51.2f;
	
	material_ironman.emissive_color[0] = 0.1f;
	material_ironman.emissive_color[1] = 0.1f;
	material_ironman.emissive_color[2] = 0.0f;
	material_ironman.emissive_color[3] = 1.0f;

	/*glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_TIGER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/tiger/tiger_tex2.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
}

void draw_ironman(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(ironman_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * ironman_n_triangles);
	glBindVertexArray(0);
}

void cleanup_ironman(void) {
	glDeleteVertexArrays(1, &ironman_VAO);
	glDeleteBuffers(1, &ironman_VBO);
}