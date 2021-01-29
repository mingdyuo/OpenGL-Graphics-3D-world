#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "Object.h"
#include <random>

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
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;


// lights in scene
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

// texture stuffs
GLuint texture_names[N_TEXTURES_USED];
int flag_texture_mapping;

int flag_translation_axis;

// fog stuffs
// you could control the fog parameters interactively: FOG_COLOR, FOG_NEAR_DISTANCE, FOG_FAR_DISTANCE   
int flag_fog;
bool addition = true;

// for tiger animation
unsigned int timestamp_scene = 0; // the global clock in the scene
int flag_tiger_animation, flag_polygon_fill;
int cur_frame_spider = 0, cur_frame_ben = 0, cur_frame_wolf = 0;
int rotation_angle = 0;

// callbacks
float PRP_distance_scale[6] = { 0.5f, 1.0f, 2.5f, 5.0f, 10.0f, 20.0f };
float PRP = 1.0f;
std::vector<Object*> objects;
Object bus(OBJ_BUS);
Object bike(OBJ_BIKE);
Object ironman(OBJ_IRONMAN);
Dynamic_Obj spider(OBJ_SPIDER);
Dynamic_Obj ben(OBJ_BEN);
Dynamic_Obj wolf(OBJ_WOLF);

bool zoom_inout = false;
Camera camera;
glm::vec3 ironman_positions[] = {
	glm::vec3(230.0f, 0.0f, -240.0f),
	glm::vec3(-111.0f, 0.0f, -310.0f),
	glm::vec3(-40.0f,0.0f,120.0f),
	glm::vec3(-100.0f,0.0f,-220.0f),
	glm::vec3(-170.0f,0.0f,340.0f),
	glm::vec3(-110.0f,0.0f,67.0f),
	glm::vec3(-450.0f,0.0f,-280.0f),
	glm::vec3(-330.0f,0.0f,177.0f),
	glm::vec3(-200.0f,0.0f,200.0f),
	glm::vec3(-240.0f,0.0f,100.0f),
	glm::vec3(900.0f,0.0f,67.0f),
	glm::vec3(450.0f,0.0f,-280.0f),
	glm::vec3(330.0f,0.0f,177.0f),
	glm::vec3(200.0f,0.0f,200.0f),
	glm::vec3(100.0f,0.0f,-220.0f),
	glm::vec3(170.0f,0.0f,340.0f),
	glm::vec3(450.0f,0.0f,-280.0f),
	glm::vec3(330.0f,0.0f,177.0f),
};

void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0 / 225.0, 0 / 225.0, 0 / 225.0, 0.5);
	glUseProgram(h_ShaderProgram_simple);
	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);
	//draw_bike();
	glUseProgram(0);

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
	glUseProgram(0);

	if (addition) {
		glUseProgram(h_ShaderProgram_TXPS);
		set_material_floor2();
		glUniform1i(loc_texture, TEXTURE_ID_FLOOR2);
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-500.0f, 0.0f, 500.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 700.0f, 1000.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
		draw_floor2();
		glUseProgram(0);

		glUseProgram(h_ShaderProgram_TXPS);
		set_material_floor2();
		glUniform1i(loc_texture, TEXTURE_ID_FLOOR2);
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-500.0f, 0.0f, -500.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 700.0f, 1000.0f));
		ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
		draw_floor2();
		glUseProgram(0);
	}
	



	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 99);



	
	//ironman.set_material();
	ironman.scale = glm::vec3(30.0f, 30.0f, 30.0f);
	ironman.model_rotate_angle = 90.0f;
	for (auto pos : ironman_positions) {
		ironman.position = pos;
		ironman.draw();
	}


	//float angle;
	float ironman_scaling = 15.0f;
	bike.scale = glm::vec3(ironman_scaling, ironman_scaling, ironman_scaling);
	bike.model_rotate_angle = -90.0f;
	if (bike.position[0] < -600.0f) {
		bike.position[0] = ben.position[0];
		bike.position[2] = ben.position[2];
		bike.velocity = 0;
	}
	if (dis(gen) < 25 && bike.position[2] > -300.0f) {
		bike.velocity += 0.3f;
		bike.position[2] += bike.velocity;
		bike.model_rotate_angle += 10.0f;
	}
	else if (dis(gen) < 50 && bike.position[2] > -300.0f) {
		bike.velocity += 0.6f;
		bike.position[2] += bike.velocity;
		bike.model_rotate_angle += 5.0f;
	}
	else if (dis(gen) < 70 && bike.position[2] < 300.0f) {
		bike.velocity += 0.4f;
		bike.position[2] -= bike.velocity;
		bike.model_rotate_angle += -15.0f;
	}
	else if (bike.position[2] < 300.0f) {
		bike.velocity += 0.7f;
		bike.position[2] -= bike.velocity;
		bike.model_rotate_angle += -5.0f;
	}
	bike.position[0] -= bike.velocity;
	bike.draw();

	glUseProgram(h_ShaderProgram_TXPS);
	glm::vec4 direction_modification = glm::rotate(glm::mat4(1.0f), (bike.model_rotate_angle + 90) * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
	light[3].spot_direction[0] = direction_modification[0];
	light[3].spot_direction[1] = direction_modification[1];
	light[3].spot_direction[2] = direction_modification[2];
	light[3].position[0] = bike.position[0]; light[3].position[1] = bike.position[1]; // spot light position in WC
	light[3].position[2] = bike.position[2];
	glm::vec4 position_EC = ViewMatrix * glm::vec4(light[3].position[0], light[3].position[1],
		light[3].position[2], light[3].position[3]);
	glUniform4fv(loc_light[3].position, 1, &position_EC[0]);
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[3].spot_direction[0],
		light[3].spot_direction[1], light[3].spot_direction[2]);
	glUniform3fv(loc_light[3].spot_direction, 1, &direction_EC[0]);
	glUseProgram(0);

	bus.position = glm::vec3(-395.0f, 0.0f, 135.0f);
	//bus.position = glm::vec3(-205.0f, 0.0f, 135.0f);
	bus.scale = glm::vec3(3.0f, 3.0f, 3.0f);
	bus.model_rotate_angle = 90.0f;
	bus.model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0, y = 0; i < 5; i++, y += 20) {
		bus.position[1] = float(y);
		bus.position[2] = 135.0f;
		bus.draw();
		bus.position[2] = -135.0f;
		bus.draw();
	}
	for (int z = -135; z <= 135; z += 20) {
		bus.position[2] = float(z);
		bus.draw();
	}



	static glm::vec3 spider_move = glm::vec3(1.0f, 0.0f, 0.0f);
	if (dis(gen) % 13 == 0) {
		switch (dis(gen) % 4) {
		case 0: spider_move = glm::vec3(1.0f, 0.0f, 0.0f); break;
		case 1: spider_move = glm::vec3(-1.0f, 0.0f, 0.0f); break;
		case 2: spider_move = glm::vec3(0.0f, 0.0f, 1.0f); break;
		case 3: spider_move = glm::vec3(0.0f, 0.0f, -1.0f); break;
		}
	}
	spider.position += spider_move * 3.0f;
	if (spider.position[0] < -500.0f || spider.position[0] > -250.0f
		|| spider.position[2] < -250.0f || spider.position[2] > 250.0f) {
		spider.position = glm::vec3(-250.0f, 0.0f, 0.0f);
	}
	spider.model_rotate_angle = 180.0f;
	spider.model_rotate_axis = glm::vec3(1.0f, 0.0f, 0.0f);
	spider.draw();


	float add_z = sinf((timestamp_scene % 720) * TO_RADIAN) * 70.0f;
	float add_x = cosf((timestamp_scene % 360) * TO_RADIAN) * 70.0f;
	if ((timestamp_scene % 720 - 90) > 360) ben.position = glm::vec3(160.0f + add_x, 10.0f, 70.0f - add_z);
	else ben.position = glm::vec3(160.0f + add_x, 10.0f, -70.0f + add_z);
	
	ben.scale = glm::vec3(100.0f, 100.0f, 100.0f);
	ben.model_rotate_angle = 180.0f;
	ben.model_rotate_axis = glm::vec3(0.7f, 0.0f, 0.7f);

	ben.draw();

	wolf.position = glm::vec3(-400.0f, 0.0f, -420.0f);
	wolf.model_rotate_angle = 0.0f;
	wolf.model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	wolf.scale = glm::vec3(100.0f, 100.0f, 100.0f);
	for (int i = 0; i < 900; i += 40) {
		float y = cosf(((timestamp_scene - i) % 360) * TO_RADIAN);
		if (y > 0) y *= 80.0f;
		else y *= 15.0f;
		wolf.position[0] = -400.0f + float(i);
		wolf.position[1] = y;
		wolf.draw();
	}
	wolf.position = glm::vec3(-400.0f, 0.0f, 450.0f);
	wolf.model_rotate_angle = 180.0f;
	wolf.model_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	wolf.scale = glm::vec3(100.0f, 100.0f, 100.0f);
	
	for (int i = 0; i < 900; i += 40) {
		float y = cosf(((timestamp_scene + i) % 360) * TO_RADIAN);
		if (y > 0) y *= 80.0f;
		else y *= 15.0f;
		wolf.position[0] = -400.0f + float(i);
		wolf.position[1] = y;
		wolf.draw();
	}
	

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
		glutTimerFunc(25, timer_scene, 0);
}


void reshape(int width, int height) {
	//float aspect_ratio;

	glViewport(0, 0, width, height);
	
	//aspect_ratio = (float) width / height;
	camera.aspect_ratio = (float)width / height;
	//ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, aspect_ratio, 100.0f, 20000.0f);
	ProjectionMatrix = glm::perspective(camera.fovy * TO_RADIAN, camera.aspect_ratio, camera.near_c, camera.far_c);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glutPostRedisplay();
}

void cleanup(void) {
	cleanup_axis();
	cleanup_floor();
	cleanup_floor2();


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

void init_objects() {
	spider.position = glm::vec3(-250.0f, 2.0f, 0.0f);
	spider.scale = glm::vec3(44.0f, 44.0f, 44.0f);

	ben.position = glm::vec3(50.0f, 2.0f, 30.0f);
	ben.model_rotate_angle = 180.0f;
	ben.model_rotate_axis = glm::vec3(1.0f, 0.0f, 0.0f);
	ben.scale = glm::vec3(50.0f, 50.0f, 50.0f);

}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 5.4.Tiger_Texture_PS_GLSL";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', 'a', 't', 'f', 'c', 'p', 'd', 'y', 'u', 'i', 'o', 'ESC'"  };
	
	init_objects();
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