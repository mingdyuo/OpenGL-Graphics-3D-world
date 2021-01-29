#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"
#include "externs.h"



Object::Object(OBJ_TYPE type) {
	obj_type = type;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	world_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	model_rotate_angle = 0.0f;
	world_rotate_angle = 0.0f;
	velocity = 0.0f;
	direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	switch (obj_type) {
	case OBJ_BUS:
		obj_filename = "Data/static_objects/bus_vnt.geom"; 
		texture_filename = "Data/barcode.jpg";
		texture_id = TEXTURE_ID_BUS;
		break;
	case OBJ_BIKE:
		obj_filename = "Data/static_objects/bike_vnt.geom"; 
		texture_filename = "Data/ball.jpg";
		texture_id = TEXTURE_ID_BIKE;
		break;
	case OBJ_IRONMAN:
		obj_filename = "Data/static_objects/ironman_vnt.geom"; 
		texture_filename = "Data/brick.jpg";
		texture_filename2 = "Data/dynamic_objects/tiger/tiger_tex2.jpg";
		texture_id2 = TEXTURE_ID_IRONMAN2;
		texture_id = TEXTURE_ID_IRONMAN;
		break;
	default:
		break;
	}
}

void Object::set_material() {
	glUniform4fv(loc_material.ambient_color, 1, material_obj.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_obj.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_obj.specular_color);
	glUniform1f(loc_material.specular_exponent, material_obj.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_obj.emissive_color);
}

void Object::prepare() {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, obj_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, obj_filename.c_str());
	obj_n_triangles = read_geometry(&obj_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	obj_n_total_triangles += obj_n_triangles;


	// initialize vertex buffer object
	glGenBuffers(1, &obj_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, obj_VBO);
	glBufferData(GL_ARRAY_BUFFER, obj_n_total_triangles * 3 * n_bytes_per_vertex, obj_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(obj_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &obj_VAO);
	glBindVertexArray(obj_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, obj_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	switch (obj_type) {
	case OBJ_BUS:
		color_bus(material_obj); break;
	case OBJ_BIKE:
		color_bike(material_obj); break;
	case OBJ_IRONMAN:
		color_ironman(material_obj); 
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

		glActiveTexture(GL_TEXTURE0 + texture_id2);
		glBindTexture(GL_TEXTURE_2D, texture_names[texture_id2]);

		My_glTexImage2D_from_file(texture_filename2);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	default:
		break;
	}

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_names[texture_id]);

	//My_glTexImage2D_from_file("Data/grass_tex.jpg");
	My_glTexImage2D_from_file(texture_filename);
	//printf("texture : %s\n", texture_filename);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}



void Object::draw() {
	glUseProgram(h_ShaderProgram_TXPS);
	set_material();
	
	if (obj_type == OBJ_IRONMAN && flag_texture_mapping == 2) {
		glUniform1i(loc_texture, texture_id2);
	}
	else glUniform1i(loc_texture, texture_id);

	ModelViewMatrix = glm::rotate(ViewMatrix, world_rotate_angle * TO_RADIAN, world_rotate_axis);
	ModelViewMatrix = glm::translate(ModelViewMatrix, position);
	ModelViewMatrix = glm::rotate(ModelViewMatrix, model_rotate_angle * TO_RADIAN, model_rotate_axis);
	ModelViewMatrix = glm::scale(ModelViewMatrix, scale);
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));


	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glFrontFace(GL_CW);

	glBindVertexArray(obj_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * obj_n_triangles);
	glBindVertexArray(0);
	glUseProgram(0);
}

Dynamic_Obj::~Dynamic_Obj() {
	delete[] obj_n_triangles;
	delete[] obj_vertex_offset;
	delete[] obj_vertices;
}

Dynamic_Obj::Dynamic_Obj(OBJ_TYPE type) : Object(type) {
	obj_type = type;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	world_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	model_rotate_angle = 0.0f;
	world_rotate_angle = 0.0f;
	switch (obj_type) {
	case OBJ_WOLF: 
		obj_filename = "Data/dynamic_objects/wolf/wolf_%02d_vnt.geom"; 
		texture_filename = "Data/horse.jpg";
		n_frames = N_WOLF_FRAMES;
		obj_cur_frame = &cur_frame_wolf;
		texture_id = TEXTURE_ID_WOLF;
		break;
	case OBJ_BEN:
		obj_filename = "Data/dynamic_objects/ben/ben_vn%d%d.geom"; 
		texture_filename = "Data/dynamic_objects/ben/human_tex.jpg";
		n_frames = N_BEN_FRAMES;
		obj_cur_frame = &cur_frame_ben;
		texture_id = TEXTURE_ID_BEN;
		break;
	case OBJ_SPIDER:
		obj_filename = "Data/dynamic_objects/spider/spider_vnt_%d%d.geom";
		texture_filename = "Data/dynamic_objects/spider/Spinnen_Bein_tex_COLOR_.jpg";
		n_frames = N_SPIDER_FRAMES;
		obj_cur_frame = &cur_frame_spider;
		texture_id = TEXTURE_ID_SPIDER;
		break;
	default:
		break;
	}
	obj_n_triangles = new int[n_frames];
	obj_vertex_offset = new int[n_frames];
	obj_vertices = new GLfloat * [n_frames];
}



void Dynamic_Obj::draw() {
	glUseProgram(h_ShaderProgram_TXPS);
	set_material();
	glUniform1i(loc_texture, texture_id);

	ModelViewMatrix = glm::rotate(ViewMatrix, world_rotate_angle * TO_RADIAN, world_rotate_axis);
	ModelViewMatrix = glm::translate(ModelViewMatrix, position);
	ModelViewMatrix = glm::rotate(ModelViewMatrix, model_rotate_angle * TO_RADIAN, model_rotate_axis);
	ModelViewMatrix = glm::scale(ModelViewMatrix, scale);
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glFrontFace(GL_CW);

	glBindVertexArray(obj_VAO);
	glDrawArrays(GL_TRIANGLES, obj_vertex_offset[*obj_cur_frame], 3 * obj_n_triangles[*obj_cur_frame]);
	glBindVertexArray(0);
	glUseProgram(0);


}

void Dynamic_Obj::prepare() {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, obj_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;


	for (i = 0; i < n_frames; i++) {
		sprintf(filename, obj_filename.c_str(), i / 10, i % 10);
		obj_n_triangles[i] = read_geometry(&obj_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		obj_n_total_triangles += obj_n_triangles[i];

		if (i == 0)
			obj_vertex_offset[i] = 0;
		else
			obj_vertex_offset[i] = obj_vertex_offset[i - 1] + 3 * obj_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &obj_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, obj_VBO);
	glBufferData(GL_ARRAY_BUFFER, obj_n_total_triangles * n_bytes_per_vertex, NULL, GL_STATIC_DRAW);

	for (i = 0; i < n_frames; i++)
		glBufferSubData(GL_ARRAY_BUFFER, obj_vertex_offset[i] * n_bytes_per_vertex,
			obj_n_triangles[i] * n_bytes_per_triangle, obj_vertices[i]);
	
	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < n_frames; i++)
		free(obj_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &obj_VAO);
	glBindVertexArray(obj_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, obj_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	switch (obj_type) {
	case OBJ_SPIDER:
		color_spider(material_obj); break;
	case OBJ_BEN:
		color_ben(material_obj); break;
	case OBJ_WOLF:
		color_wolf(material_obj); break;
	default:
		break;
	}

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_names[texture_id]);

	My_glTexImage2D_from_file(texture_filename);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


}

void color_bus(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.14725f;
	material_obj.ambient_color[1] = 0.1995f;
	material_obj.ambient_color[2] = 0.1745f;
	material_obj.ambient_color[3] = 1.0f;

	material_obj.diffuse_color[0] = 0.75164f;
	material_obj.diffuse_color[1] = 0.10648f;
	material_obj.diffuse_color[2] = 0.12648f;
	material_obj.diffuse_color[3] = 1.0f;

	/*material_obj.diffuse_color[0] = 0.75164f;
	material_obj.diffuse_color[1] = 0.60648f;
	material_obj.diffuse_color[2] = 0.22648f;
	material_obj.diffuse_color[3] = 1.0f;*/

	material_obj.specular_color[0] = 0.728281f;
	material_obj.specular_color[1] = 0.255802f;
	material_obj.specular_color[2] = 0.166065f;
	material_obj.specular_color[3] = 1.0f;

	material_obj.specular_exponent = 51.2f;


	material_obj.emissive_color[0] = 0.0f;
	material_obj.emissive_color[1] = 0.0f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;
	/*material_obj.emissive_color[0] = 0.1f;
	material_obj.emissive_color[1] = 0.1f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;*/
}

void color_bike(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.14725f;
	material_obj.ambient_color[1] = 0.1995f;
	material_obj.ambient_color[2] = 0.1745f;
	material_obj.ambient_color[3] = 1.0f;

	material_obj.diffuse_color[0] = 0.75164f;
	material_obj.diffuse_color[1] = 0.60648f;
	material_obj.diffuse_color[2] = 0.22648f;
	material_obj.diffuse_color[3] = 1.0f;

	material_obj.specular_color[0] = 0.728281f;
	material_obj.specular_color[1] = 0.655802f;
	material_obj.specular_color[2] = 0.466065f;
	material_obj.specular_color[3] = 1.0f;

	material_obj.specular_exponent = 51.2f;

	material_obj.emissive_color[0] = 0.2f;
	material_obj.emissive_color[1] = 0.2f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;
}

void color_ironman(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.24725f;
	material_obj.ambient_color[1] = 0.1995f;
	material_obj.ambient_color[2] = 0.0745f;
	material_obj.ambient_color[3] = 1.0f;

	material_obj.diffuse_color[0] = 0.75164f;
	material_obj.diffuse_color[1] = 0.60648f;
	material_obj.diffuse_color[2] = 0.22648f;
	material_obj.diffuse_color[3] = 1.0f;

	material_obj.specular_color[0] = 0.728281f;
	material_obj.specular_color[1] = 0.655802f;
	material_obj.specular_color[2] = 0.466065f;
	material_obj.specular_color[3] = 1.0f;

	material_obj.specular_exponent = 51.2f;

	material_obj.emissive_color[0] = 0.0f;
	material_obj.emissive_color[1] = 0.0f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;
}

void color_spider(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.2f;
	material_obj.ambient_color[1] = 0.2f;
	material_obj.ambient_color[2] = 0.2f;
	material_obj.ambient_color[3] = 0.0f;

	material_obj.diffuse_color[0] = 0.75164f;
	material_obj.diffuse_color[1] = 0.60648f;
	material_obj.diffuse_color[2] = 0.22648f;
	material_obj.diffuse_color[3] = 0.0f;

	material_obj.specular_color[0] = 0.4f;
	material_obj.specular_color[1] = 0.4f;
	material_obj.specular_color[2] = 0.4f;
	material_obj.specular_color[3] = 0.0f;

	material_obj.specular_exponent = 37.254902f;

	material_obj.emissive_color[0] = 0.0f;
	material_obj.emissive_color[1] = 0.0f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 0.0f;
}

void color_ben(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.0f;
	material_obj.ambient_color[1] = 0.0f;
	material_obj.ambient_color[2] = 0.0f;
	material_obj.ambient_color[3] = 1.0f;

	material_obj.diffuse_color[0] = 1.0f;
	material_obj.diffuse_color[1] = 1.0f;
	material_obj.diffuse_color[2] = 1.0f;
	material_obj.diffuse_color[3] = 1.0f;

	material_obj.specular_color[0] = 0.198791f;
	material_obj.specular_color[1] = 0.0783166f;
	material_obj.specular_color[2] = 0.104556f;
	material_obj.specular_color[3] = 1.0f;

	material_obj.specular_exponent = 50.0f;

	material_obj.emissive_color[0] = 0.0f;
	material_obj.emissive_color[1] = 0.0f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;
}

void color_wolf(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.2f;
	material_obj.ambient_color[1] = 0.2f;
	material_obj.ambient_color[2] = 0.2f;
	material_obj.ambient_color[3] = 0.0f;

	material_obj.diffuse_color[0] = 0.026142f;
	material_obj.diffuse_color[1] = 0.022739f;
	material_obj.diffuse_color[2] = 0.020186f;
	material_obj.diffuse_color[3] = 0.0f;

	material_obj.specular_color[0] = 0.0f;
	material_obj.specular_color[1] = 0.0f;
	material_obj.specular_color[2] = 0.0f;
	material_obj.specular_color[3] = 0.0f;

	material_obj.specular_exponent = 0.0f;

	material_obj.emissive_color[0] = 0.0f;
	material_obj.emissive_color[1] = 0.0f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 0.0f;
}