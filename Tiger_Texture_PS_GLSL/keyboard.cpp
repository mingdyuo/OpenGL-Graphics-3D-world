#define _CRT_SECURE_NO_WARNINGS

#include "externs.h"
#include "camera.h"


void keyboard(unsigned char key, int x, int y) {
	static int flag_cull_face = 0;
	static int PRP_distance_level = 0;
	static int flag_floor_mag_filter = 0, flag_floor_min_filter = 0;
	static int flag_tiger_mag_filter = 0, flag_tiger_min_filter = 0;
	static bool move_positive_direction = true;
	static bool rotate_negative_direction = true;

	glm::vec4 position_EC;
	glm::vec3 direction_EC;

	if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int)(key - '0');

		glUseProgram(h_ShaderProgram_TXPS);
		light[light_ID].light_on = 1 - light[light_ID].light_on;
		glUniform1i(loc_light[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(0);

		glutPostRedisplay();
		return;
	}

	switch (key) {
	case 'a': // toggle the animation effect.
		flag_tiger_animation = 1 - flag_tiger_animation;
		if (flag_tiger_animation) {
			glutTimerFunc(100, timer_scene, 0);
			fprintf(stdout, "^^^ Animation mode ON.\n");
		}
		else
			fprintf(stdout, "^^^ Animation mode OFF.\n");
		break;
	case 'f':
		flag_fog = 1 - flag_fog;
		if (flag_fog)
			fprintf(stdout, "^^^ Fog mode ON.\n");
		else
			fprintf(stdout, "^^^ Fog mode OFF.\n");
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_fog, flag_fog);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 't':
		flag_texture_mapping = 1 - flag_texture_mapping;
		if (flag_texture_mapping)
			fprintf(stdout, "^^^ Texture mapping ON.\n");
		else
			fprintf(stdout, "^^^ Texture mapping OFF.\n");
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_texture_mapping, flag_texture_mapping);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 'y': // Change the floor texture's magnification filter.
		flag_floor_mag_filter = (flag_floor_mag_filter + 1) % 2;
		glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_FLOOR);
		if (flag_floor_mag_filter == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			fprintf(stdout, "^^^ Mag filter for floor: GL_NEAREST.\n");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			fprintf(stdout, "^^^ Mag filter for floor: GL_LINEAR.\n");
		}
		glutPostRedisplay();
		break;
	case 'u': // Change the floor texture's minification filter.
		flag_floor_min_filter = (flag_floor_min_filter + 1) % 3;
		glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_FLOOR);
		if (flag_floor_min_filter == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			fprintf(stdout, "^^^ Min filter for floor: GL_NEAREST.\n");
		}
		else if (flag_floor_min_filter == 1) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			fprintf(stdout, "^^^ Min filter for floor: GL_LINEAR.\n");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			fprintf(stdout, "^^^ Min filter for floor: GL_LINEAR_MIPMAP_LINEAR.\n");
		}
		glutPostRedisplay();
		break;
	case 'i': // Change the tiger texture's magnification filter.

		PRP -= 0.02f;
		if (PRP < 0.3f) PRP = 0.3f;
		{
			glm::vec3 temp_pos = camera.pos;
			/*if (PRP < 0.4f) PRP = 0.4f;*/
			camera.pos *= PRP;
			camera.set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			camera.pos = temp_pos;
		}
		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;

		//flag_tiger_mag_filter = (flag_tiger_mag_filter + 1) % 2;
		////glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
		//if (flag_tiger_mag_filter == 0) {
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//	fprintf(stdout, "^^^ Mag filter for tiger: GL_NEAREST.\n");
		//}
		//else {
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	fprintf(stdout, "^^^ Mag filter for tiger: GL_LINEAR.\n");
		//}
		//glutPostRedisplay();
		//break;
	case 'o': // Change the tiger texture's minification filter.

		/*camera.fovy += 0.02f;
		if (camera.fovy > 17.0f) camera.fovy = 17.0f;*/
		PRP += 0.02f;

		{
			glm::vec3 temp_pos = camera.pos;
			/*if (PRP < 0.4f) PRP = 0.4f;*/
			camera.pos *= PRP;
			camera.set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			camera.pos = temp_pos;
		}


		/*PRP_distance_level = (PRP_distance_level + 1) % 6;
		fprintf(stdout, "^^^ Distance level = %d.\n", PRP_distance_level);

		PRP += 0.02f;
		if (PRP > 17.0f) PRP = 17.0f;

		ViewMatrix = glm::lookAt(PRP * glm::vec3(500.0f, 300.0f, 500.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;

		//flag_tiger_min_filter = (flag_tiger_min_filter + 1) % 3;
		////glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
		//if (flag_tiger_min_filter == 0) {
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//	fprintf(stdout, "^^^ Min filter for tiger: GL_NEAREST.\n");
		//}
		//else if (flag_tiger_min_filter == 1) {
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	fprintf(stdout, "^^^ Min filter for tiger: GL_LINEAR.\n");
		//}
		//else {
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	fprintf(stdout, "^^^ Min filter for tiger: GL_LINEAR_MIPMAP_LINEAR.\n");
		//}
		//glutPostRedisplay();
		//break;
	case 'c':
		flag_cull_face = (flag_cull_face + 1) % 3;
		switch (flag_cull_face) {
		case 0:
			fprintf(stdout, "^^^ Face culling OFF.\n");
			glDisable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 1: // cull back faces;
			fprintf(stdout, "^^^ BACK face culled.\n");
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 2: // cull front faces;
			fprintf(stdout, "^^^ FRONT face culled.\n");
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		}
		break;
	case 'd':
		PRP_distance_level = (PRP_distance_level + 1) % 6;
		fprintf(stdout, "^^^ Distance level = %d.\n", PRP_distance_level);
		PRP += 0.02f;
		if (PRP > 15.0f) PRP = 15.0f;
		else if (PRP < 0.2f) PRP = 0.2f;

		ViewMatrix = glm::lookAt(PRP * glm::vec3(500.0f, 300.0f, 500.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		/*ViewMatrix = glm::lookAt(PRP_distance_scale[PRP_distance_level] * glm::vec3(500.0f, 300.0f, 500.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 'p':
		flag_polygon_fill = 1 - flag_polygon_fill;
		if (flag_polygon_fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'm':
		move_positive_direction = !move_positive_direction;
		break;
	case ',': // move toward u axis
		if (move_positive_direction)
			camera.pos += camera.uaxis;
		else
			camera.pos -= camera.uaxis;
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case '.': // move toward v axis
		if (move_positive_direction)
			camera.pos += camera.vaxis;
		else
			camera.pos -= camera.vaxis;
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case '/': // move toward n axis
		if (move_positive_direction)
			camera.pos += camera.naxis;
		else
			camera.pos -= camera.naxis;
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glUseProgram(h_ShaderProgram_TXPS);
		// Must update the light 1's geometry in EC.
		position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
			light[1].position[2], light[1].position[3]);
		glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
		direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0],
			light[1].spot_direction[1], light[1].spot_direction[2]);
		glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 'k':
		rotate_negative_direction = !rotate_negative_direction;
		break;
	case 'l': // rotate by uaxis
		{
			float angle = rotate_negative_direction ? -1.0f : 1.0f;
			glm::vec4 temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.uaxis) * glm::vec4(camera.naxis, 1.0f);
			camera.naxis = glm::vec3(temp[0], temp[1], temp[2]);
			temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.uaxis) * glm::vec4(camera.vaxis, 1.0f);
			camera.vaxis = glm::vec3(temp[0], temp[1], temp[2]);
		}
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case ';': // rotate by vaxis
		{
			float angle = rotate_negative_direction ? -1.0f : 1.0f;
			glm::vec4 temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.vaxis) * glm::vec4(camera.naxis, 1.0f);
			camera.naxis = glm::vec3(temp[0], temp[1], temp[2]);
			temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.vaxis) * glm::vec4(camera.uaxis, 1.0f);
			camera.uaxis = glm::vec3(temp[0], temp[1], temp[2]);
		}
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '\'': // rotate by naxis
		{
			float angle = rotate_negative_direction ? -1.0f : 1.0f;
			glm::vec4 temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.naxis) * glm::vec4(camera.vaxis, 1.0f);
			camera.vaxis = glm::vec3(temp[0], temp[1], temp[2]);
			temp = glm::rotate(glm::mat4(1.0f), angle * TO_RADIAN, camera.naxis) * glm::vec4(camera.uaxis, 1.0f);
			camera.uaxis = glm::vec3(temp[0], temp[1], temp[2]);
		}
		camera.set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}
}