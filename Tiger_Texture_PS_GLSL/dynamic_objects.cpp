#define _CRT_SECURE_NO_WARNINGS
#include "dynamic_objects.h"
#include "externs.h"

extern loc_Material_Parameters loc_material;

// spider object
GLuint spider_VBO, spider_VAO;
int spider_n_triangles[N_SPIDER_FRAMES];
int spider_vertex_offset[N_SPIDER_FRAMES];
GLfloat* spider_vertices[N_SPIDER_FRAMES];

Material_Parameters material_spider;
extern int cur_frame_spider;
extern int cur_frame_ben;
extern int cur_frame_wolf;

void prepare_spider(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, spider_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_SPIDER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/spider/spider_vnt_%d%d.geom", i / 10, i % 10);
		spider_n_triangles[i] = read_geometry(&spider_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		spider_n_total_triangles += spider_n_triangles[i];

		if (i == 0)
			spider_vertex_offset[i] = 0;
		else
			spider_vertex_offset[i] = spider_vertex_offset[i - 1] + 3 * spider_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &spider_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glBufferData(GL_ARRAY_BUFFER, spider_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_SPIDER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, spider_vertex_offset[i] * n_bytes_per_vertex,
			spider_n_triangles[i] * n_bytes_per_triangle, spider_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_SPIDER_FRAMES; i++)
		free(spider_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &spider_VAO);
	glBindVertexArray(spider_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_spider.ambient_color[0] = 1.0f;
	material_spider.ambient_color[1] = 1.0f;
	material_spider.ambient_color[2] = 1.0f;
	material_spider.ambient_color[3] = 0.0f;

	material_spider.diffuse_color[0] = 0.75164f;
	material_spider.diffuse_color[1] = 0.60648f;
	material_spider.diffuse_color[2] = 0.22648f;
	material_spider.diffuse_color[3] = 0.0f;

	material_spider.specular_color[0] = 0.4f;
	material_spider.specular_color[1] = 0.4f;
	material_spider.specular_color[2] = 0.4f;
	material_spider.specular_color[3] = 0.0f;

	material_spider.specular_exponent = 37.254902f;

	material_spider.emissive_color[0] = 0.0f;
	material_spider.emissive_color[1] = 0.0f;
	material_spider.emissive_color[2] = 0.0f;
	material_spider.emissive_color[3] = 0.0f;


	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_SPIDER);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_SPIDER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/spider/Spinnen_Bein_tex_COLOR_.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void draw_spider(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(spider_VAO);
	glDrawArrays(GL_TRIANGLES, spider_vertex_offset[cur_frame_spider], 3 * spider_n_triangles[cur_frame_spider]);
	glBindVertexArray(0);
}

// ben object
GLuint ben_VBO, ben_VAO;
int ben_n_triangles[N_BEN_FRAMES];
int ben_vertex_offset[N_BEN_FRAMES];
GLfloat* ben_vertices[N_BEN_FRAMES];

Material_Parameters material_ben;

void prepare_ben(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, ben_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_BEN_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/ben/ben_vn%d%d.geom", i / 10, i % 10);
		ben_n_triangles[i] = read_geometry(&ben_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		ben_n_total_triangles += ben_n_triangles[i];

		if (i == 0)
			ben_vertex_offset[i] = 0;
		else
			ben_vertex_offset[i] = ben_vertex_offset[i - 1] + 3 * ben_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &ben_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glBufferData(GL_ARRAY_BUFFER, ben_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_BEN_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, ben_vertex_offset[i] * n_bytes_per_vertex,
			ben_n_triangles[i] * n_bytes_per_triangle, ben_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_BEN_FRAMES; i++)
		free(ben_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &ben_VAO);
	glBindVertexArray(ben_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_ben.ambient_color[0] = 0.0f;
	material_ben.ambient_color[1] = 0.0f;
	material_ben.ambient_color[2] = 0.0f;
	material_ben.ambient_color[3] = 1.0f;

	material_ben.diffuse_color[0] = 1.0f;
	material_ben.diffuse_color[1] = 1.0f;
	material_ben.diffuse_color[2] = 1.0f;
	material_ben.diffuse_color[3] = 1.0f;

	material_ben.specular_color[0] = 0.198791f;
	material_ben.specular_color[1] = 0.0783166f;
	material_ben.specular_color[2] = 0.104556f;
	material_ben.specular_color[3] = 1.0f;

	material_ben.specular_exponent = 50.0f;

	material_ben.emissive_color[0] = 0.0f;
	material_ben.emissive_color[1] = 0.0f;
	material_ben.emissive_color[2] = 0.0f;
	material_ben.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_BEN);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_BEN]);

	My_glTexImage2D_from_file("Data/dynamic_objects/ben/human_tex.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void draw_ben(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(ben_VAO);
	glDrawArrays(GL_TRIANGLES, ben_vertex_offset[cur_frame_ben], 3 * ben_n_triangles[cur_frame_ben]);
	glBindVertexArray(0);
}

// wolf object
GLuint wolf_VBO, wolf_VAO;
int wolf_n_triangles[N_WOLF_FRAMES];
int wolf_vertex_offset[N_WOLF_FRAMES];
GLfloat* wolf_vertices[N_WOLF_FRAMES];

Material_Parameters material_wolf;

void prepare_wolf(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, wolf_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_WOLF_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/wolf/wolf_%02d_vnt.geom", i);
		wolf_n_triangles[i] = read_geometry(&wolf_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		wolf_n_total_triangles += wolf_n_triangles[i];

		if (i == 0)
			wolf_vertex_offset[i] = 0;
		else
			wolf_vertex_offset[i] = wolf_vertex_offset[i - 1] + 3 * wolf_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &wolf_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, wolf_VBO);
	glBufferData(GL_ARRAY_BUFFER, wolf_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_WOLF_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, wolf_vertex_offset[i] * n_bytes_per_vertex,
			wolf_n_triangles[i] * n_bytes_per_triangle, wolf_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_WOLF_FRAMES; i++)
		free(wolf_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &wolf_VAO);
	glBindVertexArray(wolf_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, wolf_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_wolf.ambient_color[0] = 0.0f;
	material_wolf.ambient_color[1] = 0.0f;
	material_wolf.ambient_color[2] = 0.0f;
	material_wolf.ambient_color[3] = 0.0f;

	material_wolf.diffuse_color[0] = 0.026142f;
	material_wolf.diffuse_color[1] = 0.022739f;
	material_wolf.diffuse_color[2] = 0.020186f;
	material_wolf.diffuse_color[3] = 0.0f;

	material_wolf.specular_color[0] = 0.0f;
	material_wolf.specular_color[1] = 0.0f;
	material_wolf.specular_color[2] = 0.0f;
	material_wolf.specular_color[3] = 0.0f;

	material_wolf.specular_exponent = 0.0f;

	material_wolf.emissive_color[0] = 0.0f;
	material_wolf.emissive_color[1] = 0.0f;
	material_wolf.emissive_color[2] = 0.0f;
	material_wolf.emissive_color[3] = 0.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_WOLF);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_ID_WOLF]);

	My_glTexImage2D_from_file("Data/dynamic_objects/wolf/Wolf_Fur.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void draw_wolf(void) {
	glFrontFace(GL_CW);


	glBindVertexArray(wolf_VAO);
	glDrawArrays(GL_TRIANGLES, wolf_vertex_offset[cur_frame_wolf], 3 * wolf_n_triangles[cur_frame_wolf]);
	glBindVertexArray(0);
}


#pragma region SetMaterial
void set_material_spider(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_spider.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_spider.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_spider.specular_color);
	glUniform1f(loc_material.specular_exponent, material_spider.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_spider.emissive_color);
}
void set_material_ben(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_ben.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_ben.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_ben.specular_color);
	glUniform1f(loc_material.specular_exponent, material_ben.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_ben.emissive_color);
}
void set_material_wolf(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_wolf.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_wolf.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_wolf.specular_color);
	glUniform1f(loc_material.specular_exponent, material_wolf.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_wolf.emissive_color);
}
#pragma endregion