#define _CRT_SECURE_NO_WARNINGS
#include "Object.h"



Object::Object(OBJ_TYPE type) {
	obj_type = type;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	world_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	model_rotate_angle = 0.0f;
	world_rotate_angle = 0.0f;
	switch (obj_type) {
	case OBJ_BUS:
		obj_filename = "Data/static_objects/bus_vnt.geom"; break;
	case OBJ_BIKE:
		obj_filename = "Data/static_objects/bike_vnt.geom"; break;
	case OBJ_IRONMAN:
		obj_filename = "Data/static_objects/ironman_vnt.geom"; break;
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
	/*glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	switch (obj_type) {
	case OBJ_BUS:
		color_bus(material_obj); break;
	case OBJ_BIKE:
		color_bike(material_obj); break;
	case OBJ_IRONMAN:
		color_ironman(material_obj); break;
	default:
		break;
	}

	
}

void Object::draw() {
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
}
//
//Dynamic_Obj::Dynamic_Obj(OBJ_TYPE type, int* cur_frame) {
//	obj_type = type;
//	position = glm::vec3(0.0f, 0.0f, 0.0f);
//	scale = glm::vec3(1.0f, 1.0f, 1.0f);
//	model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
//	world_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
//	model_rotate_angle = 0.0f;
//	world_rotate_angle = 0.0f;
//	obj_cur_frame = cur_frame;
//	switch (obj_type) {
//	case OBJ_WOLF: 
//		obj_filename = "Data/dynamic_objects/wolf/wolf_%02d_vnt.geom"; break;
//	case OBJ_BEN:
//
//	case OBJ_SPIDER:
//
//	default:
//		break;
//	}
//}
//
//void Dynamic_Obj::draw() {
//	ModelViewMatrix = glm::rotate(ViewMatrix, world_rotate_angle * TO_RADIAN, world_rotate_axis);
//	ModelViewMatrix = glm::translate(ModelViewMatrix, position);
//	ModelViewMatrix = glm::rotate(ModelViewMatrix, model_rotate_angle * TO_RADIAN, model_rotate_axis);
//	ModelViewMatrix = glm::scale(ModelViewMatrix, scale);
//	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
//	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
//
//	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
//	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
//	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
//
//	glFrontFace(GL_CW);
//
//	glBindVertexArray(obj_VAO);
//	glDrawArrays(GL_TRIANGLES, 0, 3 * obj_n_triangles);
//	glBindVertexArray(0);
//}
//
//

void color_bus(Material_Parameters& material_obj) {
	material_obj.ambient_color[0] = 0.24725f;
	material_obj.ambient_color[1] = 0.1995f;
	material_obj.ambient_color[2] = 0.0745f;
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


	material_obj.emissive_color[0] = 0.4f;
	material_obj.emissive_color[1] = 0.4f;
	material_obj.emissive_color[2] = 0.4f;
	material_obj.emissive_color[3] = 1.0f;
	/*material_obj.emissive_color[0] = 0.1f;
	material_obj.emissive_color[1] = 0.1f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;*/
}

void color_bike(Material_Parameters& material_obj) {
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

	material_obj.emissive_color[0] = 0.1f;
	material_obj.emissive_color[1] = 0.1f;
	material_obj.emissive_color[2] = 0.0f;
	material_obj.emissive_color[3] = 1.0f;
}