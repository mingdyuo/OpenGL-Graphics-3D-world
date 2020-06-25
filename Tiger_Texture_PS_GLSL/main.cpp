#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

GLuint h_ShaderProgram_simple, h_ShaderProgram_TXPS; // handles to shader programs

// for simple shaders
GLint loc_ModelViewProjectionMatrix_simple, loc_primitive_color;

// for Phong Shading (Textured) shaders
#define NUMBER_OF_LIGHT_SUPPORTED 4 
GLint loc_global_ambient_color;
loc_light_Parameters loc_light[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material;
GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
GLint loc_texture, loc_flag_texture_mapping, loc_flag_fog;

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
//#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
//#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ViewMatrix, ProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

// lights in scene
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

// texture stuffs
#define N_TEXTURES_USED 4
#define TEXTURE_ID_FLOOR 0
#define TEXTURE_ID_WOLF 1
#define TEXTURE_ID_SPIDER 2
#define TEXTURE_ID_BEN 3
GLuint texture_names[N_TEXTURES_USED];
int flag_texture_mapping;

//extern GLuint rectangle_VBO, rectangle_VAO;
//extern GLuint axes_VBO, axes_VAO;


// fog stuffs
// you could control the fog parameters interactively: FOG_COLOR, FOG_NEAR_DISTANCE, FOG_FAR_DISTANCE   
int flag_fog;

// for tiger animation
unsigned int timestamp_scene = 0; // the global clock in the scene
int flag_tiger_animation, flag_polygon_fill;
int cur_frame_spider = 0, cur_frame_ben = 0, cur_frame_wolf;
int rotation_angle = 0;

// callbacks
float PRP_distance_scale[6] = { 0.5f, 1.0f, 2.5f, 5.0f, 10.0f, 20.0f };

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(h_ShaderProgram_simple);
	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);
	//draw_bike();

	glUseProgram(h_ShaderProgram_TXPS);
  	set_material_floor();
	glUniform1i(loc_texture, TEXTURE_ID_FLOOR);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-500.0f, 0.0f, 500.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_floor();



	set_material_floor();
	glUniform1i(loc_texture, TEXTURE_ID_FLOOR);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-500.0f, 0.0f, 500.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 700.0f, 1000.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_floor();


	set_material_ben();
	glUniform1i(loc_texture, TEXTURE_ID_BEN);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.0f, 0.0f, -50.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(60.0f, -50.0f, 60.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_bus();

	glUseProgram(h_ShaderProgram_simple);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	glUseProgram(0);

	glutSwapBuffers();
}

void timer_scene(int value) {
	timestamp_scene = (timestamp_scene + 1) % UINT_MAX;
	//cur_frame_tiger = timestamp_scene % N_TIGER_FRAMES;
	cur_frame_spider = timestamp_scene % N_SPIDER_FRAMES;
	cur_frame_ben = timestamp_scene % N_BEN_FRAMES;
	cur_frame_wolf = timestamp_scene % N_WOLF_FRAMES;
	rotation_angle = (timestamp_scene % 360)*TO_RADIAN;
	glutPostRedisplay();
	if (flag_tiger_animation)
		glutTimerFunc(100, timer_scene, 0);
}

void keyboard(unsigned char key, int x, int y) {
	static int flag_cull_face = 0;
	static int PRP_distance_level = 0;
	static int flag_floor_mag_filter = 0, flag_floor_min_filter = 0;
	static int flag_tiger_mag_filter = 0, flag_tiger_min_filter = 0;

	glm::vec4 position_EC;
	glm::vec3 direction_EC;

	if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int) (key - '0');

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
		flag_tiger_mag_filter = (flag_tiger_mag_filter + 1) % 2;
		//glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
		if (flag_tiger_mag_filter == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			fprintf(stdout, "^^^ Mag filter for tiger: GL_NEAREST.\n");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			fprintf(stdout, "^^^ Mag filter for tiger: GL_LINEAR.\n");
		}
		glutPostRedisplay();
		break;
	case 'o': // Change the tiger texture's minification filter.
		flag_tiger_min_filter = (flag_tiger_min_filter + 1) % 3;
		//glActiveTexture(GL_TEXTURE0 + TEXTURE_ID_TIGER);
		if (flag_tiger_min_filter == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			fprintf(stdout, "^^^ Min filter for tiger: GL_NEAREST.\n");
		}
		else if (flag_tiger_min_filter == 1) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			fprintf(stdout, "^^^ Min filter for tiger: GL_LINEAR.\n");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			fprintf(stdout, "^^^ Min filter for tiger: GL_LINEAR_MIPMAP_LINEAR.\n");
		}
		glutPostRedisplay();
		break;
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

		ViewMatrix = glm::lookAt(PRP_distance_scale[PRP_distance_level] * glm::vec3(500.0f, 300.0f, 500.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}
}

void reshape(int width, int height) {
	float aspect_ratio;

	glViewport(0, 0, width, height);
	
	aspect_ratio = (float) width / height;
	ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, aspect_ratio, 100.0f, 20000.0f);

	glutPostRedisplay();
}

void cleanup(void) {
	cleanup_axis();

	cleanup_floor();

	cleanup_bus();

	cleanup_bike();


	glDeleteTextures(N_TEXTURES_USED, texture_names);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}






void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 5.4.Tiger_Texture_PS_GLSL";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', 'a', 't', 'f', 'c', 'p', 'd', 'y', 'u', 'i', 'o', 'ESC'"  };

	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}